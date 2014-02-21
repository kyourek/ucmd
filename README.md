ucmd
========
**Command-line parser and application framework for C and embedded.**

**ucmd** provides a command-line interface for microcontrollers or any system with a C compiler. It's accompanied by a unit-test framework that can run on the same systems to which it is deployed. The repository contains some simple *Hello-World* example projects.

**ucmd** is designed to be portable and rely on minimal dependencies. It works on [Arduino](http://www.arduino.cc/) and [Maple](http://leaflabs.com/), and has been tested and used extensively on other ARM devices. It uses no dynamic memory allocation, so it is usable on systems that don't implement or define `malloc`. Instead, it relies on configurable header files that define the number of available commands, switches, and arguments. These numbers can be reduced when working in memory restrictive environments.

**ucmd** is developed in an object-oriented style with strong naming conventions. A typical function definition contains a type name, a method name, and accepts a pointer to an instance on which the method works. For example, if the struct `uc_dog` is defined, it might have a function definition that looks like this: `uc_dog_bark(uc_dog *p);`

Usage
========

A program that requires one command with multiple arguments and switches could build it's requirements like this:

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

`cmd` is a pointer to a function of type `uc_cmd_line_opt_func`. This function is called when the application receives the command "**cmd**". Help text is defined at the time of command/switch/argument creation, and can be seen when the application receives the command "**help**":

    cmd [arg] [-sw1] [s1-a1] [s1-a2] -sw2 s2-a1
    cmd: A simple example command.
        [arg]: Help text for the command argument.
        [-sw1]: This is Switch 1.
                [s1-a1]: Info about switch 1, arg 1.
                [s1-a2]: Some help for switch 1, arg 2.
        -sw2: And this is the second switch, which is required.
                s2-a1: It has a required argument.

Above, `-sw2` was created as a `required` switch. Therefore, if it is not supplied, the application responds with an error message:

    cmd
    Invalid switch: the switch "-sw2" is required.
    
`-sw2` also defines it's argument as required. So, if `-sw2` is supplied but its argument is not, a different error message is sent:

    cmd -sw2
    Invalid switch argument: the argument "s2-a1" is required for switch "-sw2".

The argument passed to the `cmd` function is a pointer to an object of type `uc_cmd_line` that facilitates parsing the command's switches and arguments. It also provides a means of sending string responses to the command.
