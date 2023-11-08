#include "rogue/io/rl_config.h"

#include "rogue/utility/rl_string_tools.h"
#include "rogue/math/rl_math.h"

#include "kinc/system.h"
#include "kinc/io/filewriter.h"
#include "kinc/io/filereader.h"

#include <stdio.h>
#include <ctype.h>

typedef enum rl_config_token_type_e
{
    rl_config_token_type_e_null,
    rl_config_token_type_e_parameter,
    rl_config_token_type_e_number,

    rl_config_token_type_e_max_count

} rl_config_token_type_e;

typedef struct rl_config_token_t
{
    rl_config_token_type_e type;
    char const * token;
    size_t length;

} rl_config_token_t;

typedef struct rl_config_parser_t
{
    char const * file;
    size_t file_size;
    size_t to_process;

    int64_t tokens_length;
    rl_config_token_t tokens[RL_CONFIG_MAX_TOKENS];

} rl_config_parser_t;

//=========================================================

void rl_config_parser_step(rl_config_parser_t * parser)
{
    parser->file += 1;
    parser->to_process -= 1;
}

bool rl_config_parser_has_data(rl_config_parser_t const * parser)
{
    return (parser->to_process > 0);
}

void rl_config_parser_push_token(rl_config_parser_t * parser, rl_config_token_t token)
{
    if (parser->tokens_length < RL_CONFIG_MAX_TOKENS)
    {
        parser->tokens[parser->tokens_length] = token;
        parser->tokens_length += 1;
    }
    else
    {
        RL_ASSERT(0);
    }
}

static inline bool rl_config_parser_cursor_is_number(rl_config_parser_t const * parser)
{
    return (isdigit(*parser->file));
}

static inline bool rl_config_parser_cursor_is_param(rl_config_parser_t const * parser)
{
    return (isalpha(*parser->file) || *parser->file == '_' || *parser->file == ':');
}

static inline char const * rl_config_parser_cursor_location(rl_config_parser_t const * parser)
{
    return parser->file;
}

void rl_config_parser_read_tokens(rl_config_parser_t * parser)
{
    while (rl_config_parser_has_data(parser))
    {
        rl_config_token_t token = {0};

        if (rl_config_parser_cursor_is_param(parser))
        {
            token.token = rl_config_parser_cursor_location(parser);
            token.type = rl_config_token_type_e_parameter;

            while (rl_config_parser_has_data(parser) && rl_config_parser_cursor_is_param(parser))
            {
                token.length += 1;
                rl_config_parser_step(parser);
            }

            rl_config_parser_push_token(parser, token);
        }
        else if (rl_config_parser_cursor_is_number(parser))
        {
            token.token = rl_config_parser_cursor_location(parser);
            token.type = rl_config_token_type_e_number;

            while (rl_config_parser_has_data(parser) && rl_config_parser_cursor_is_number(parser))
            {
                token.length += 1;
                rl_config_parser_step(parser);
            }

            rl_config_parser_push_token(parser, token);
        }
        else
        {
            rl_config_parser_step(parser);
        }
    }
}

int rl_config_token_value_to_integer(rl_config_token_t token)
{
    return RL_CAST(int, rl_string_tools_string_to_integer(token.token, token.length));
}

bool rl_config_parameter_matches(rl_config_token_t token, char const * parameter)
{
    //NOTE(zshoals): Maybe unsafe? Parameter is null terminated, but token.token is not. However, we have the length limit...
    return (strncmp(parameter, token.token, token.length) == 0);
}

bool rl_config_is_valid_parameter(rl_config_parser_t * parser, int64_t token_index)
{
    if ( (token_index + 1) < parser->tokens_length)
    {
        rl_config_token_t t1 = parser->tokens[token_index];
        rl_config_token_t t2 = parser->tokens[token_index + 1];

        if (t1.type != rl_config_token_type_e_parameter) { return false; }
        if (t2.type != rl_config_token_type_e_number) { return false; }

        return true;
    }

    return false;
}

bool rl_config_save_to_disk(rl_config_t config)
{
    char buffer[RL_CONFIG_BUFFER_CAPACITY] = {0};

    char const * format = 
    "window_width:%d\n"
    "window_height:%d\n"
    "wants_vertical_sync:%d\n"
    "wants_borderless_fullscreen:%d\n";

    int written = snprintf(
        &(buffer[0]), 
        RL_CONFIG_BUFFER_CAPACITY, 
        format, 
        config.window_width, 
        config.window_height, 
        RL_CAST(int, config.wants_vertical_sync),
        RL_CAST(int, config.wants_borderless_fullscreen)
    );

    if ( (written < RL_CONFIG_BUFFER_CAPACITY) && (written > 0) )
    {
        kinc_file_writer_t writer = {0};
        
        if (kinc_file_writer_open(&writer, RL_CONFIG_PATH))
        {
            kinc_file_writer_write(&writer, &(buffer[0]), written);
            kinc_file_writer_close(&writer);

            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

rl_config_t rl_config_load_from_disk_or_apply_defaults(void)
{
    //Note(zshoals):> Always set defaults, just in case of missing parameters in the config file
    rl_config_t config = {0};
    {
        config.window_width = RL_CONFIG_DEFAULT_WINDOW_WIDTH;
        config.window_height = RL_CONFIG_DEFAULT_WINDOW_HEIGHT;
        config.wants_vertical_sync = RL_CONFIG_DEFAULT_VERTICAL_SYNC;
        config.wants_borderless_fullscreen = RL_CONFIG_DEFAULT_BORDERLESS_FULLSCREEN;
    }

    kinc_file_reader_t reader = {0};
    if (kinc_file_reader_open(&reader, RL_CONFIG_PATH, KINC_FILE_TYPE_SAVE))
    {
        size_t file_size = kinc_file_reader_size(&reader);
        if (file_size > 0 && file_size < RL_CONFIG_BUFFER_CAPACITY)
        {
            char buffer[RL_CONFIG_BUFFER_CAPACITY] = {0};
            kinc_file_reader_read(&reader, &(buffer[0]), file_size);

            rl_config_parser_t parser = {0};
            {
                parser.file = &(buffer[0]);
                parser.file_size = file_size;
                parser.to_process = file_size;
            }

            rl_config_parser_read_tokens(&parser);

            for (int64_t i = 0; i < parser.tokens_length;)
            {
                if (rl_config_is_valid_parameter(&parser, i))
                {
                    rl_config_token_t param = parser.tokens[i];
                    rl_config_token_t numeric = parser.tokens[i + 1];

                    if (rl_config_parameter_matches(param, "window_width:"))
                    {
                        int window_width = rl_config_token_value_to_integer(numeric);
                        window_width = rl_math_clamp_i(window_width, RL_CONFIG_MIN_WINDOW_WIDTH, RL_CONFIG_MAX_WINDOW_WIDTH);

                        config.window_width = window_width;
                    }
                    else if (rl_config_parameter_matches(param, "window_height:"))
                    {
                        int window_height = rl_config_token_value_to_integer(numeric);
                        window_height = rl_math_clamp_i(window_height, RL_CONFIG_MIN_WINDOW_HEIGHT, RL_CONFIG_MAX_WINDOW_HEIGHT);

                        config.window_height = window_height;
                    }
                    else if (rl_config_parameter_matches(param, "wants_vertical_sync:"))
                    {
                        bool vsync_enabled = RL_CAST(bool, rl_config_token_value_to_integer(numeric));
                        config.wants_vertical_sync = vsync_enabled;
                    }
                    else if (rl_config_parameter_matches(param, "wants_borderless_fullscreen:"))
                    {
                        bool borderless_enabled = RL_CAST(bool, rl_config_token_value_to_integer(numeric));
                        config.wants_borderless_fullscreen = borderless_enabled;
                    }

                    i += 2;
                }
                else
                {
                    i += 1;
                }
            }
        }

        kinc_file_reader_close(&reader);
    }


    return config;
}
