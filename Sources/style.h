



//Structs
    typedef struct rl_generic_array_t 
    {
        int a;
    } 
    rl_generic_array_t;

//Enums and Enum Values
    typedef enum rl_graphics_render_mode_e
    {
        rl_graphics_render_mode_e_transparency,
        rl_graphics_render_mode_e_opaque,
        rl_graphics_render_mode_e_max_count
    } 
    rl_graphics_render_mode_e;

//Unions
    typedef union rl_graphics_object_u
    {
        int a;
        float b;
    } 
    rl_graphics_object_u;

//Functions
    static inline void rl_graphics_perform_action(float x, float y);

    static inline void rl_graphics_perform_action(float x, float y)
    {
        float nothing = x + y;
    }

    int rl_graphics_do_thing(float z)
    {
        int out = RL_CAST(int, z);

        return out;
    }

//Constant Values ("invariant" is used instead of the word "constant"; invariant is less likely to be used as part of a type name)
    #define rl_math_invariant_pi 3.14
    double const rl_math_invariant_tau = 6.28;
    double const rl_graphics_invariant_magic_value = 69;

//Macro functions, special defines that are not really meant to be used as constant math values
    #define RL_FEATURE_ENABLED 1
    #define RL_CAST(TARGET_TYPE, VARIABLE) (TARGET_TYPE)(VARIABLE)

//Increment, Decrement
    //++ and -- disallowed. Instead...
    int var = 1;
    var += 1;
    var -= 1;