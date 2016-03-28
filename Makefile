help:
	@echo '======================================================'
	@echo 'Makefile for NodeFinderC                              '
	@echo '                                                      '
	@echo 'Usage:                                                '
	@echo '    make install     compile NodeFinderC to executable'
	@echo '    make clean       do clean job                     '
	@echo '======================================================'

install:
	@echo 'Compile NodeFinderC to executable...'
	gcc nodefinderc.c -o bin/nodefinderc-latest

clean:
	@echo 'Start cleaning...'
	rm *.o
