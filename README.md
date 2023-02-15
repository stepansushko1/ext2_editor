# ext2_editor

Project for UCU Operating Systems course. 

Basic simple UI ext2 filesystem edior based on QtCreator 6.

Implemented functions:

* Visual part of ext2
* Changing the characteristics of the inode
* Visulise content of inode
* Block data change
* Changing the inode table
* Back up the corrupted file
* Copying files (writing to a directory)

Layout:

* data_folder - contains iso, img you want to edit. 
* data_folder/mount_f - for checking files on mounted iso, img.
* test - contains a project
* test/main.[cpp,h,ui] - contains main function to start ui.
* test/main_window.[cpp,h,ui] - main UI window constains list of all n group blocks.
* test/superblockwindow[cpp,h,ui] - UI with displayed inforamtion about superblock.
* test/groupdescriptors[cpp,h,ui] - UI with displayed inforamtion about group descriptors.
* test/datablockbitmap[cpp,h,ui] - UI with displayed data blocks bitmap in binary representation.
* test/inodebitmap[cpp,h,ui] - UI with displayed inode blocks bitmap in binary representation.
* test/inodetable[cpp,h,ui] - contains list of all n inodes.
* test/inodetableoneinode[cpp,h,ui] - cointains information about particular inode, that can be changed by user, adding and removing datablocks, displaying content of inode (for example, inode 2 - entries of root dir).
* test/inode_funtionality[cpp,h,ui] - contains copy and move functions of content of the inode by specific number.
