#include <string.h>
#include "ucmdexample.h"

#define NAME_LENGTH 50

typedef struct {
    char first_name[NAME_LENGTH];
    char last_name[NAME_LENGTH];
    char full_name[NAME_LENGTH * 2 + 1]; 
} 
AppState;

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

static const char *name(ucCmd *cmd, void *state) {
    ucTok *name_arg;
    AppState *app_state;

    app_state = (AppState*)state;

    if (ucCmd_find_switch(cmd, "-first")) {
        name_arg = ucCmd_get_switch_arg(cmd, "-first");
        copy_name(name_arg, app_state->first_name);
    }

    if (ucCmd_find_switch(cmd, "-last")) {
        name_arg = ucCmd_get_switch_arg(cmd, "-last");
        copy_name(name_arg, app_state->last_name);
    }

    return ucCmd_format_response(
        cmd, 
        "Name: %s",
        format_name(app_state)
    );
}

static const char *say(ucCmd *cmd, void *state) {
    ucTok *phrase; 
    AppState *app_state;

    phrase = ucCmd_get_arg(cmd);
    app_state = (AppState*)state;

    ucCmd_respond(cmd, ucCmd_format_response(
        cmd,
        "%s, %s!",
        phrase,
        format_name(app_state)
    ));

    return NULL;
}

void uc_hello_world(ucCmd_TransmitFunc *transmit, ucCmd_ReceiveFunc *receive) {

    AppState app_state = { 0 };                     /* This is the program's state object that gets */
                                                    /* passed to the various command functions.     */
    ucCmdOpt *commands = 
        ucCmdOpt_create(                            /* Create a command that calls the function     */
            name,                                   /* 'name'. Its purpose is to set the user's     */
                                                    /* name.                                        */
            
            &app_state,                             /* This pointer, to the object that maintains   */
                                                    /* the program's state, is passed to the func-  */
                                                    /* tion 'name' when it is called.               */
            
            "name",                                 /* This command is invoked when the user types  */
                                                    /* 'name'.                                      */
            
            "Sets the user's name",                 /* This description of the command is shown     */
                                                    /* when the user types 'help'.                  */

            NULL,                                   /* No arguments are required (or allowed) for   */
                                                    /* the command 'name', so NULL is passed here.  */
                                                    
            ucSwitchOpt_create(                     /* Create the switch '-first' for the command   */
                "-first",                           /* 'name' (and gives it a description). This    */
                "Sets the user's first name",       /* switch is optional.                          */
                
                ucArgOpt_create_required(           /* Create the argument 'first-name' for the     */
                    "first-name",                   /* switch '-first'. This argument is required   */
                    "The first-name of the user",   /* if the switch is used.                       */
                
                NULL),                              /* No other arguments are required (or allowed) */
                                                    /* for the switch '-first', so NULL is passed   */
                                                    /* here.                                        */
            
            ucSwitchOpt_create(                     /* Another switch is created for the user's     */
                "-last",                            /* last name. This switch is very similar to    */
                "Sets the user's last name",        /* the switch '-first'.                         */
                ucArgOpt_create_required(
                    "last-name",
                    "The last-name of the user",
                NULL),
            
            NULL)),                                 /* That was the last switch for the command     */
                                                    /* 'name', so we pass NULL here.                */
        
        ucCmdOpt_create(                            /* Create another command called 'say'. This    */
            say,                                    /* command will say a phrase to the user.       */
            &app_state,
            "say",
            "Says a phrase",
            
            ucArgOpt_create_required(               /* There is only one argument supplied to this  */
                "phrase",                           /* command, and it is required. It is the       */
                "The phrase to say",                /* phrase to say to the user.                   */
                NULL),
            NULL,
        
        NULL));                                     /* NULL is passed here to signal that there are */
                                                    /* no more commands.                            */

    ucApp *app = ucApp_create();                    /* Create an instance of the application type.  */

    ucCmd *cmd = ucApp_get_cmd(app);                /* Get the object used by the application that  */
                                                    /* receives commands from the user (and sends   */
                                                    /* responses).                                  */
    
    ucCmd_set_receive(cmd, receive);                /* Set callback functions on that object that   */
    ucCmd_set_transmit(cmd, transmit);              /* allow transmission and reception of data.    */
                                                    /* These callbacks will be specific to the      */
                                                    /* platform on which the app is running.        */

    ucApp_set_name(                                 /* Give the application a name. This name will  */
        app,                                        /* be displayed on the banner when the appli-   */
        "ucmd example -- Hello, World!");           /* cation starts.                               */
                   

    ucApp_set_cmd_opt(app, commands);               /* Set the command-line options (which were     */
                                                    /* created above) that are available to the     */
                                                    /* application.                                 */
                                                    
    ucApp_run(app);                                 /* Start the application. This call will return */
                                                    /* when the 'quit' command is received.         */
    
    ucApp_destroy(app);                             /* Clean up the resources that were explicitly  */
    ucCmdOpt_destroy_chain(commands);               /* created.                                     */
}
