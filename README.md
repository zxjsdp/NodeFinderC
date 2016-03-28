NodeFinderC
===========

Introduction
------------

NodeFinderC is C implementation of NodeFinder program(Originally implemented with Python).


Usage
-----

Basic usage:

    /path/to/nodefinder -i input_tree.nwk -c config_file.txt -o output_tree.nwk

You can read help message with `-h` argument:

    /path/to/nodefinder -h

Tips:

- `-i`, `-c`, and `-o` must be specified.
- `-i` means input_tree.nwk, which is a Newick format tree file;
- `-c` means config_file.txt, please refer to **Configuaration File Syntax** part;
- `-o` means output_tree.nwk, which will be the new generated output Newick format tree file.


Configuration File Syntax
-------------------------

Overview:

    name_a, name_b, calibration_infomation_1;
    name_c, name_d, calibration_infomation_2;
    name_a, name_b, clade_label_information;
    name, branch_label_information;
    ..., ..., ...;

Tips:

- Elemenets in each line should be seperated by `,`;
- Each config line must end with a `;`;
- Spaces, newlines, tabs are acceptable in config file, the program will ignore them;
