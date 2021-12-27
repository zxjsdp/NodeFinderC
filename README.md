NodeFinderC
===========

![NodeFinderC GIF](resources/NodeFinderC.gif)

Introduction
------------

NodeFinderC is C implementation of NodeFinder program (Originally implemented with Python).

Precompiled binary executables are available here: [Windows & Linux & MacOS executables](https://github.com/zxjsdp/NodeFinderC/releases/).

If you prefer programs with GUI (Graphical User Interface), please check the GUI version of NodeFinder: [NodeFinderGUI](https://github.com/zxjsdp/NodeFinderGUI).


How to compile
--------------

Make install (recommended):

    make install

Or:

    gcc nodefinderc.c -o bin/nodefinderc-latest

Or:

    cd scripts && bash compile_to_bin.sh

After compiling, you can find NodeFinderC executable in `bin/` folder.

Usage
-----

Basic usage:

    bin/nodefinderc-latest -i sample/input_tree.nwk -c sample/sample_config.cfg -o sample/output_tree.nwk

You can read help message with `-h` argument:

    /path/to/nodefinder -h

You can read version information with `-v` argument:

    /path/to/nodefinder -v

Tips:

- `-i`, `-c`, and `-o` must be specified.
- `-i` means input_tree.nwk, which is a Newick format tree file;
- `-c` means config_file.txt, please refer to [**Configuaration File Syntax**](https://github.com/zxjsdp/NodeFinderC#configuration-file-syntax) part;
- `-o` means output_tree.nwk, which will be the new generated output Newick format tree file.
- `-h` will print help message then quit, whether other arguments were specified or not.
- `-v` will print version info then quit, whether other arguments were specified or not.


Sample Output
-------------

    =======================================================================
    [COMMAND USED]

        bin/nodefinderc-latest -i sample/input_tree.nwk -c sample/sample_config.cfg -o sample/output_tree.nwk 

        -i   Input tree file:    sample/input_tree.nwk
        -c   Config file:        sample/sample_config.cfg
        -o   Output tree file:   sample/output_tree.nwk

    [VALID CALIBRATIONS]

        1 | Tri10178, Ana08-05, #2
        2 | AnaMs2, Fis7414, >0.22<0.33
        3 | Ana1tu, Ana1616, $1


    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Tri10178, Ana08-05, #2
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    [Ancestor List]:  1385 1376 1368 1359 1349 1348 1347 186 114
    [Ancestor List]:  1385 1376 1368 1359
    [Common]:  1359

    [Insert]:  90)))))))),Ana08-05),Ana1616),Ana133),An
    [Insert]:                   ->||<-
    [Insert]:                 Insert Here


    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    AnaMs2, Fis7414, >0.22<0.33
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    [Ancestor List]:  1387 1378 1370 1359 1349 1348 1347 186 185 166 165 164
    [Ancestor List]:  1387 1378 1370 1359 1349 1348 1347 1346 1260 1233 1232 1113 1057 271
    [Common]:  1347

    [Insert]:  99,Anabaena_90)))))))),Ana08-05)#2,Ana16
    [Insert]:                   ->||<-
    [Insert]:                 Insert Here


    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Ana1tu, Ana1616, $1
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    [Ancestor List]:  1397 1388 1380 1369 1359 1358 1347 1346 1345
    [Ancestor List]:  1397 1388 1380
    [Common]:  1380

    [Insert]:  Ana08-05)#2,Ana1616),Ana133),Ana1984);
    [Insert]:                   ->||<-
    [Insert]:                 Insert Here

    (Nostoc_azollae_0708,(((((AnaXP35,(AnaJAHN,(Ana1446,Anabaena_cylindrica_PCC_7122))),(Nos8941,(((Tri10178,Tri10269),((Anabaena_sp._PCC_7108,(A.NMC-1,(Tri2001,AnaMs2))),(Ana001,AnaKVJ17))),(((Nostoc_punctiforme_PCC_73102,((((Fis7414,(Calothrix_PCC_6303,Calothrix_PCC_7103)),((((Microcoleus_PCC_7113,Coleofasciculus_chthonoplastes_PCC_7420),(Chroococcidiopsis_thermalis_PCC_7203,Gloeocapsa_PCC_7428)),((Chamaesiphon_PCC_6605,Crinalium_epipsammum_PCC_9333),(Dactylococcopsis_salina_PCC_8305,(Cyanothece_PCC_7424,((Cyanothece_PCC_8801,Cyanothece_PCC_8802),(Cyanobacterium_aponinum_PCC_10605,Synechocystis_PCC_6803)))))),((((((Prochlorococcus_marinus_MIT_9313,Synechococcus_sp._WH_8109),(Prochlorococcus_marinus_MIT_9301,Prochlorococcus_marinus_MIT_9211)),Synechococcus_elongatus_PCC_6301),(Gloeobacter_violaceus_PCC_7421,((Ecoli_K12_DH10B,(Chlamydia_trachomatis_434,Spirochaeta_thermophila_DSM_6192)),Met2661))),(Acaryochloris_marina_MBIC11017,(Thermosynechococcus_elongatus_BP-1,Cyanothece_PCC_7425))),(Synechococcus_PCC_7502,Trichodesmium_erythraeum_IMS101)))),(Calothrix_PCC_7507,Cylindrospermum_stagnale_PCC_7417)),(NosDe1,(Nostoc_PCC_7107,(Nostoc_PCC_7524,(Nostoc_PCC_7120,(((178,177),(175,174)),Anabaena_variabilis_ATCC_29413))))))),((Nod7804,Glo14),Cal4356)),(Ana1tu,(AnaBIR2,(((Ana9705,((1255,1250),Aph2012)),Aph-flos),(Ana299,Anabaena_90))))))>0.22<0.33)),Ana08-05)#2,Ana1616)$1,Ana133),Ana1984);


    =======================================================================

    Write output to file: sample/output_tree.nwk


Configuration File Syntax
-------------------------

Overview:

    name_a, name_b, calibration_infomation_1;
    name_c, name_d, calibration_infomation_2;
    name_a, name_b, clade_label_information;
    name, branch_label_information;
    ..., ..., ...;

Tips:

- Elemenets in each line should be seperated by commas: `,` (halfwidth);
- Each config line must end with a semicolon: `;` (halfwidth);
- Spaces, newlines, and tabs are acceptable in config file, the program will ignore them;
- You can comment a line (make it ignored) by adding `#` at the beginning of the line;

Implementation
--------------

Given two species, this program finds all ancestor nodes for each species by
using stack (to exclude other monophyletic group) and parenthesis. Then compare
these two ancestor node list and find the index of most recent common
ancestor nodes.

For example:

    List of ancestor nodes index:
    species1:     [57, 62, 73, 102, 162, 214, 258]
                                ^    ^    ^    ^
                                |    |    |    |
    species2: [39, 48, 81, 94, 102, 162, 214, 258]

    Then 102 will be the index of most recent common ancestor node.
