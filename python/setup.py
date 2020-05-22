import setuptools

setuptools.setup(
    name="qsdn",
    version="v0.0.1",
    author="Shawn.Pringle",
    author_email="shawn.pringle@gmail.com",
    description="Python Qt5 Classes that Display and Parse Numbers using Standard Decimal Notation",
    long_description="""This module gives you classes to produce, read, and validate text in Standard Decimal Notation.

		Often other libraries will use a kind of scientific notation, or will neglect to use group separators where appropriate.""",
    long_description_content_type="text/plain",
    url="https://github.com/shawnpringle/separatorsafterdecimal",
    packages=setuptools.find_packages(),
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    install_requires=[
          'PyQt5',
    ],
    download_url="https://github.com/shawnpringle/separatorsafterdecimal/archive/v0.0.1.tar.gz",
)

