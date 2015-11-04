#include <string.h>
#include "ucmdexample.h"

#define NAME_LENGTH 50

struct AppState {
    char first_name[NAME_LENGTH];
    char last_name[NAME_LENGTH];
    char full_name[NAME_LENGTH * 2 + 1];
};

typedef struct AppState AppState;

static const char *format_name(AppState *state) {
    int i, j, len_first, len_last;
    const char *last, *first;
    static const char *def = "World";

    if (!state) return def;

    first = state->first_name;
    last = state->last_name;

    len_first = strlen(first);
    len_last = strlen(last);

    if ((!len_first) && (!len_last)) return def;

    if (!len_first) return last;
    if (!len_last) return first;

    for (i = 0; i < len_first; i++) {
        state->full_name[i] = first[i];
    }
    
    state->full_name[i++] = ' ';

    for (j = 0; j < len_last; j++) {
        state->full_name[i + j] = last[j];
    }

    state->full_name[i + j] = '\0';

    return state->full_name;
}

static void copy_name(const char *source, char *dest) {
    int i, len;

    if (!source) {
        dest[0] = '\0';
        return;
    }

    if ((0 == strcmp("''", source)) || (0 == strcmp("\"\"", source))) {
        dest[0] = '\0';
        return;
    }

    len = NAME_LENGTH - 1;
    for (i = 0; i < len; i++) {
        if (!source[i]) break;
        dest[i] = source[i];
    }

    dest[i] = '\0';
}

static const char *name(ucCmdLine *cmd, void *state) {

    AppState *my_state;
    ucCmdTok *cmd_tok;
    ucArgTok *name_arg;
    ucSwitchTok *root_switch, *first_name_switch, *last_name_switch;

    my_state = (AppState*)state;
    if (!my_state) return "Oops... NULL pointer!";

    cmd_tok = ucCmdLine_get_cmd_tok(cmd);
    root_switch = ucCmdTok_get_switch(cmd_tok);

    first_name_switch = ucSwitchTok_find(root_switch, "-first");
    if (first_name_switch) {

        name_arg = ucSwitchTok_get_arg(first_name_switch);
        copy_name(ucTok_get_value(name_arg), my_state->first_name);
    }

    last_name_switch = ucSwitchTok_find(root_switch, "-last");
    if (last_name_switch) {

        name_arg = ucSwitchTok_get_arg(last_name_switch);
        copy_name(ucTok_get_value(name_arg), my_state->last_name);
    }

    return ucCmdLine_format_response(
        cmd, 
        "Name: %s",
        format_name(my_state)
    );
}

static const char *say(ucCmdLine *cmd, void *state) {
    AppState *my_state = (struct AppState*)state;

    ucCmdTok *cmd_tok = ucCmdLine_get_cmd_tok(cmd);
    ucArgTok *arg_tok = ucCmdTok_get_arg(cmd_tok);

    const char *phrase = ucTok_get_value(arg_tok);

    ucCmdLine_respond(cmd, ucCmdLine_format_response(
        cmd,
        "%s, %s!",
        phrase,
        format_name(my_state)
    ));

    return NULL;
}

void uc_hello_world(ucCmdLine_TransmitFunc *transmit, ucCmdLineApp_ReceiveFunc *receive) {
    
    /* This is the program's state object that gets
    passed to the various command functions. */
    AppState my_state = { 0 };

    ucCmdLineOpt *commands = 

        /* Creates a command 'name' that
        calls the function 'name'. Its
        purpose is to set the user's name. */
        ucCmdLineOpt_create(
            name,

            /* This pointer to the object that maintains
            the program's state is passed to the 'name'
            function when it is called. */
            &my_state,
            "name",
            "Sets the user's name",

            /* No arguments are required (or allowed)
            for the 'name' command, so NULL is passed
            here. */
            NULL,

            /* Creates a switch '-first' for the 'name'
            command. This switch is optional. */
            ucSwitchOpt_create(
                "-first",
                "Sets the user's first name",

                /* Creates an argument 'first-name' for the
                '-first' switch. This argument is required
                if the '-first' switch is used. */
                ucArgOpt_create_required(
                    "first-name",
                    "The first-name of the user",
                
                /* No other arguments are required (or allowed)
                for the '-first' switch, so NULL is passed
                here. */    
                NULL),

            /* Another switch is created for the user's
            last name. This switch is very similar to the
            '-first' switch. */
            ucSwitchOpt_create(
                "-last",
                "Sets the user's last name",
                ucArgOpt_create_required(
                    "last-name",
                    "The last-name of the user",
                NULL),

            /* That was the last switch for the 'name' command,
            so we pass NULL here. */
            NULL)),

        /* Creates another command called 'say'.
        This command will say a phrase to the user. */
        ucCmdLineOpt_create(
            say,
            &my_state,
            "say",
            "Says a phrase",

            /* There is only one argument supplied
            to this command, and it is required. It
            is the phrase to say to the user. */
            ucArgOpt_create_required(
                "phrase",
                "The phrase to say",
                NULL),
            NULL,

        /* NULL is passed here to signal that there
        are no more commands. */
        NULL));

    /* This ucCmdLineApp instance is a singleton that
    is available to run a command-line app. */
    ucCmdLineApp *app = ucCmdLineApp_create();

    /* Gets the ucCmdLine used by the app to send
    responses back to the user. */
    ucCmdLine *cmd = ucCmdLineApp_get_cmd(app);

    /* Sets callback functions on the app and command
    that allow transmission and reception of data. 
    These callbacks will be specific to the platform
    on which the app is running. */
    ucCmdLine_set_transmit(cmd, transmit);
    ucCmdLineApp_set_receive(app, receive);

    /* Starts the command-line app. The app will exit
    when it receives the 'quit' command. */
    ucCmdLineApp_run(app, commands);

    /* Clean up the resources that were explicitly
    created. */
    ucCmdLineApp_destroy(app);
    ucCmdLineOpt_destroy_chain(commands);
}
