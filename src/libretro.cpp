
#include <libretro.h>
#include <gameboycore/gameboycore.h>

#include <array>
#include <vector>

using namespace gb;

// CONSTANTS
static constexpr unsigned int DISPLAY_WIDTH = 160;
static constexpr unsigned int DISPLAY_HEIGHT = 144;


// PROTOTYPES
static void gpu_callback(const GPU::Scanline& scanline, int line);
static uint8_t convert_rgb24_to_rgb15(uint8_t c);
static void apu_callback(int16_t left, int16_t right);
static void process_input();

// VARIABLES
static GameboyCore core;
static short framebuffer[DISPLAY_WIDTH * DISPLAY_HEIGHT];
static std::vector<int16_t> audio_buffer;

static std::array<std::pair<std::size_t, Joy::Key>, 8> key_map = 
{
    {
        { RETRO_DEVICE_ID_JOYPAD_UP,     Joy::Key::UP },
        { RETRO_DEVICE_ID_JOYPAD_DOWN,   Joy::Key::DOWN },
        { RETRO_DEVICE_ID_JOYPAD_LEFT,   Joy::Key::LEFT },
        { RETRO_DEVICE_ID_JOYPAD_RIGHT,  Joy::Key::RIGHT },
        { RETRO_DEVICE_ID_JOYPAD_A,      Joy::Key::A },
        { RETRO_DEVICE_ID_JOYPAD_B,      Joy::Key::B },
        { RETRO_DEVICE_ID_JOYPAD_START,  Joy::Key::START },
        { RETRO_DEVICE_ID_JOYPAD_SELECT, Joy::Key::SELECT }
    }
};

// Frontend callbacks
static retro_environment_t environment_cb;
static retro_video_refresh_t video_cb;
static retro_audio_sample_batch_t audio_batch_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;
static retro_log_printf_t log_cb;


unsigned retro_api_version(void)
{
    return RETRO_API_VERSION;
}

void retro_init()
{
    retro_log_callback log;
    unsigned int level = 4;

    if (environment_cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &log))
    {
        log_cb = log.log;
    }
    else
    {
        log_cb = nullptr;
    }

    environment_cb(RETRO_ENVIRONMENT_SET_PERFORMANCE_LEVEL, &level);
}

void retro_deinit()
{

}


/**
    Tell libretro information about the core
*/
void retro_get_system_info(retro_system_info* info)
{
    memset(info, 0, sizeof(retro_system_info));
    info->library_name = "GameboyCore";
    info->library_version = GAMEBOYCORE_RETRO_VERSION;
    info->need_fullpath = false;
    info->valid_extensions = "gb|gbc";
}

/**
    Tell libretro audio and video information
*/
void retro_get_system_av_info(retro_system_av_info* info)
{
    memset(info, 0, sizeof(retro_system_av_info));
    info->timing.fps = 60.0f;
    info->timing.sample_rate = 44100; // standard audio sample rate
    info->geometry.base_width = DISPLAY_WIDTH;
    info->geometry.base_height = DISPLAY_HEIGHT;
    info->geometry.max_width = DISPLAY_WIDTH;
    info->geometry.max_height = DISPLAY_HEIGHT;
    info->geometry.aspect_ratio = (float)DISPLAY_HEIGHT / (float)DISPLAY_WIDTH;
}

void retro_set_environment(retro_environment_t cb)
{
    environment_cb = cb;
}

/**
    Load ROM
*/
bool retro_load_game(const retro_game_info* info)
{
    // load rom data into core
    if (info && info->data)
    {
        core.loadROM((uint8_t*)info->data, info->size);

        // set core callbacks
        core.setScanlineCallback(std::bind(gpu_callback, std::placeholders::_1, std::placeholders::_2));
        core.setAudioSampleCallback(std::bind(apu_callback, std::placeholders::_1, std::placeholders::_2));
    }

    return true;
}

bool retro_load_game_special(unsigned game_type, const struct retro_game_info *info, size_t num_info)
{ 
    return false;
}

/**
    Unload ROM
*/
void retro_unload_game(void)
{
}

void retro_reset(void)
{
}

/**
    Run
*/
void retro_run(void)
{
    core.emulateFrame();

    process_input();

    // send the current frame buffer to frontend
    video_cb(framebuffer, DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_WIDTH * sizeof(short));

    // send batched audio
    if (audio_buffer.size() > 2)
    {
        audio_batch_cb(&audio_buffer[0], audio_buffer.size() / 2);
        audio_buffer.clear();
    }
}


void process_input()
{
    // poll input
    input_poll_cb();

    // poll retro pad inputs
    for (const auto& pair : key_map)
    {
        auto value = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, pair.first);
        core.input(pair.second, value != 0);
    }
}

/**
    GPU Callback
*/
void gpu_callback(const GPU::Scanline& scanline, int line)
{
    const auto offset = DISPLAY_WIDTH * line;
    const auto size = scanline.size();

    for (auto i = 0u; i < size; ++i)
    {
        const auto& pixel = scanline[i];
        const auto r = convert_rgb24_to_rgb15(pixel.r);
        const auto g = convert_rgb24_to_rgb15(pixel.g);
        const auto b = convert_rgb24_to_rgb15(pixel.b);

        short rgb = 0x8000 | (r << 10) | (g << 5) | b;

        framebuffer[offset + i] = rgb;
    }
}

static void apu_callback(int16_t left, int16_t right)
{
    audio_buffer.push_back(left);
    audio_buffer.push_back(right);
}

/**
    Convert
*/
uint8_t convert_rgb24_to_rgb15(uint8_t c)
{
    static constexpr int MAX_15 = 0x1F;

    float current_ratio = (float)c / 255.0f;

    return (uint8_t)((float)MAX_15 * current_ratio);
}


void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb)
{
    audio_batch_cb = cb;
}

void retro_set_video_refresh(retro_video_refresh_t cb)
{
    video_cb = cb;
}

void retro_set_audio_sample(retro_audio_sample_t cb)
{
}

void retro_set_input_poll(retro_input_poll_t cb)
{
    input_poll_cb = cb;
}

void retro_set_input_state(retro_input_state_t cb)
{
    input_state_cb = cb;
}

void retro_set_controller_port_device(unsigned port, unsigned device)
{
}

size_t retro_serialize_size(void)
{
    return 0;
}

bool retro_serialize(void *data, size_t size)
{
    return false;
}

bool retro_unserialize(const void *data, size_t size)
{
    return false;
}

void retro_cheat_reset(void)
{
}

void retro_cheat_set(unsigned index, bool enabled, const char *code)
{
}

void *retro_get_memory_data(unsigned id)
{
    return NULL;
}

size_t retro_get_memory_size(unsigned id)
{
    return 0;
}

unsigned retro_get_region(void)
{
    return RETRO_REGION_PAL;
}

