Separators After Decimal
======================

These classes and later perhaps routines for purely functional languages
are for working with numbers expressed in standard decimal notation.

Often libraries for working with locales neglect to put thousand separators (commas) after the decimal place or they sometimes use scientific notation.  In the classes and routines of this package you are guaranteed to always get standard notation and separators after the decimal.  This means that commas come before and after the decimal place.  Using KDE's settings you can change the comma or decimal to any symbol you wish to use.

This library requires PyQt5 and Python 3.6-3.7.

Building and Installation
======================

For C, you can use LCC.  Look in ./c/compilers for its makefile.

For Python the PIP method for installing source code is broken.  I'm sorry about that.  You're free to paste the source files into your own source trees at least for now.  Now you should be able to run the test provided you have PyQt5 installed on your system.  Run "configure;make" on a Bash prompt.  If you are using Windows you can use MinGW.

