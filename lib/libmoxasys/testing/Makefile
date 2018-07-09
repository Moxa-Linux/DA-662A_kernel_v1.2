SUB_DIRS=log db ini net mdp
all: 
	for dir in $(SUB_DIRS); do \
		make -C $$dir ;\
	done
clean:
	for dir in $(SUB_DIRS); do \
		make -C $$dir clean;\
	done

