all: test docs

test : 
	(cd python;python ./test.py)
	
docs : doc/source/_static doc/source/_templates
	cd doc; make singlehtml latexpdf

doc/source/_static :
	mkdir -p doc/source/_static

doc/source/_templates :
	mkdir -p doc/source/_templates
	
	
.PHONY: test docs
