all: test docs

test :
	(cd python;python ./test.py && cp qsdn.py qsdn.py.bak)
	

docs doc : doc/build/latex/StandardDecimalNotation.pdf doc/build/singlehtml/index.html
	
dist : StandardDecimalNotation/dist/StandardDecimalNotation-0.0.0.tar.gz
	
doc/build/latex/StandardDecimalNotation.pdf doc/build/singlehtml/index.html : doc/source/_static doc/source/_templates python/qsdn.py
	cd doc; make singlehtml latexpdf

StandardDecimalNotation/dist/StandardDecimalNotation-0.0.0.tar.gz : doc/build/latex/StandardDecimalNotation.pdf doc/build/singlehtml/index.html python/qsdn.py LICENSE README.md python/setup.py CHANGES.txt GNUmakefile
	mkdir -p StandardDecimalNotation/docs
	cp -r doc/build/latex/StandardDecimalNotation.{pdf,tex} doc/build/singlehtml StandardDecimalNotation/docs
	cp CHANGES.txt LICENSE python/setup.py StandardDecimalNotation
	cat README.md > StandardDecimalNotation/README.txt
	echo -e 'include *.txt\nrecursive-include docs *.*\n' > StandardDecimalNotation/MANIFEST.in
	mkdir -p StandardDecimalNotation/standarddecimalnotation/test
	mkdir -p StandardDecimalNotation/standarddecimalnotation
	cp python/qsdn.py StandardDecimalNotation/standarddecimalnotation;
	cp python/test.py StandardDecimalNotation/standarddecimalnotation/test
	touch StandardDecimalNotation/standarddecimalnotation/__init__.py StandardDecimalNotation/standarddecimalnotation/test/__init__.py	
	cd StandardDecimalNotation && python setup.py sdist
	
distclean : 
	rm -r StandardDecimalNotation
	
realclean : distclean
	(cd doc;make clean)

	
doc/source/_static :
	mkdir -p doc/source/_static

doc/source/_templates :
	mkdir -p doc/source/_templates
	
	
.PHONY: test doc docs dist distclean realclean
