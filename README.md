ucmd
========

**Command-line parser and application framework for C and embedded.**

**ucmd** works on [Arduino](http://www.arduino.cc/) and [Maple](http://leaflabs.com/).

Build commands like this:

    uc_cmd_line_opt *commands = 
        uc_cmd_line_opt_create(
            cmd,
            NULL,
            "cmd",
            "A simple example command.",
            uc_arg_opt_create(
                "arg",
                "Help text for the command argument.",
            NULL),
            uc_switch_opt_create(
                "-sw1",
                "This is Switch 1.",
                uc_arg_opt_create(
                    "s1-a1",
                    "Info about switch 1, arg 1.",
                uc_arg_opt_create(
                    "s1-a2",
                    "Some help for switch 1, arg 2.",
                NULL)),
            uc_switch_opt_create_required(
                "-sw2",
                "And this is the second switch, which is required.",
                uc_arg_opt_create_required(
                    "s2-a1",
                    "It has a required argument.",
                NULL),
            NULL)),
        NULL);

And get help messages like this:

    cmd [arg] [-sw1] [s1-a1] [s1-a2] -sw2 s2-a1
    cmd: A simple example command.
        [arg]: Help text for the command argument.
        [-sw1]: This is Switch 1.
                [s1-a1]: Info about switch 1, arg 1.
                [s1-a2]: Some help for switch 1, arg 2.
        -sw2: And this is the second switch, which is required.
                s2-a1: It has a required argument.

And validation messages like this:

    cmd
    Invalid switch: the switch "-sw2" is required.
    
And this:

    cmd -sw2
    Invalid switch argument: the argument "s2-a1" is required for switch "-sw2".
