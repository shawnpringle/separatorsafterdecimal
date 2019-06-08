import setuptools

with open("README.txt", "r") as fh:
    long_description = fh.read()

setuptools.setup(
    name="qsdn",
    version="1.0.0rc1",
    author="Shawn.Pringle",
    author_email="shawn.pringle@gmail.com",
    description="Python Qt5 Classes that Display and Parse Numbers using Standard Decimal Notation",
    long_description="""These classes and later perhaps routines for purely functional languages are for working with numbers expressed in standard decimal notation.

Often libraries for working with locales neglect to put thousand separators (commas) after the decimal point or they sometimes use scientific notation. In the classes and routines of this package you are guaranteed to always get standard notation and separators after the decimal. This means that commas come before and after the decimal place. Using KDE's settings you can change the comma or decimal to any symbol you wish to use.

This library requires PyQt5 and Python 3.6-3.7.""",
    long_description_content_type="text/python",
    url="https://github.com/shawnpringle/separatorsafterdecimal",
    packages=setuptools.find_packages(),
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
)
