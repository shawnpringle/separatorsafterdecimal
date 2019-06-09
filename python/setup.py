import setuptools

with open("README.txt", "r", encoding='utf-8') as fh:
    long_description = fh.read()

setuptools.setup(
    name="qsdn",
    version="1.0.0",
    author="Shawn.Pringle",
    author_email="shawn.pringle@gmail.com",
    description="Python Qt5 Classes that Display and Parse Numbers using Standard Decimal Notation",
    long_description=long_description,
    url="https://github.com/shawnpringle/separatorsafterdecimal",
    packages=setuptools.find_packages(),
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
)
