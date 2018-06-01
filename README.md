# C---teletext
This project parses binary .m7 files to render teletext style pages, by parsing each byte with reference to a table of control codes.
Typing make in each of the three directories will build and run the contents. Source shows the key functionality of the project (mainly in teletext.c)
, with 3 example m7 files which can be selected in the Makefile. Testing thoroughly tests the code, and Extension demonstrates the beginnings 
of a m7 file authoring tool. Here, a formal grammar is defined in Backus-Naur form, before being parsed and rendered by a recursive descent parser 
in rdp.c.
