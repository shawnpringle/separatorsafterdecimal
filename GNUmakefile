all: test docs
include os.mak
VERSION=0.1.0rc1

test :
	(cd python;$(PYTHON) ./test.py && cp qsdn.py qsdn.py.bak)
	

docs doc : doc/build/latex/StandardDecimalNotation.pdf doc/build/singlehtml/index.html
	
dist : StandardDecimalNotation-${VERSION}.tar.gz
	
doc/build/latex/StandardDecimalNotation.pdf doc/build/singlehtml/index.html : doc/source/_static doc/source/_templates python/qsdn/__init__.py
	cd doc; make singlehtml latexpdf

StandardDecimalNotation-0.1.0rc1.tar.gz : doc/build/latex/StandardDecimalNotation.pdf doc/build/singlehtml/index.html python/qsdn/__init__.py LICENSE README.md python/setup.py CHANGES.txt GNUmakefile dist
	mkdir -p StandardDecimalNotation/standarddecimalnotation/docs
	cp -r doc/build/latex/StandardDecimalNotation.{pdf,tex} doc/build/singlehtml StandardDecimalNotation/standarddecimalnotation/docs
	cp CHANGES.txt LICENSE python/setup.py StandardDecimalNotation
	cat README.md > StandardDecimalNotation/README.txt
	echo -e 'include *.txt\nrecursive-include standarddecimalnotation/docs *.*\n' > StandardDecimalNotation/MANIFEST.in
	mkdir -p StandardDecimalNotation/standarddecimalnotation/test
	mkdir -p StandardDecimalNotation/standarddecimalnotation
	cp python/qsdn.py StandardDecimalNotation/standarddecimalnotation;
	cp python/test.py StandardDecimalNotation/standarddecimalnotation/test
	touch StandardDecimalNotation/standarddecimalnotation/__init__.py StandardDecimalNotation/standarddecimalnotation/test/__init__.py	
	cd StandardDecimalNotation && python setup.py sdist
	mv StandardDecimalNotation/dist/StandardDecimalNotation-0.1.0rc1.tar.gz .
	
	
distclean : 
	rm -r StandardDecimalNotation
	
realclean : distclean
	(cd doc;make clean)

	
doc/source/_static :
	mkdir -p doc/source/_static

doc/source/_templates :
	mkdir -p doc/source/_templates
	
install :
	(cd StandardDecimalNotation;[ `id -u ` == 0 ] &&  $(PYTHON) setup.py install  || $(PYTHON) setup.py install --user)	
	
.PHONY: test doc docs dist distclean realclean install
