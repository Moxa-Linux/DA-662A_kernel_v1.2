SUB_DIRS=log ini mdp db net
all: 
	for dir in $(SUB_DIRS); do \
		make -C $$dir ;\
	done
clean:
	for dir in $(SUB_DIRS); do \
		make -C $$dir clean;\
	done

