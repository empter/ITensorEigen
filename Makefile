LIBRARY_DIR=$(HOME)/ITensor
eigenpath=/usr/local/include/eigen3

APP=es

HEADERS=

CCFILES=$(APP).cc

#################################################################
#################################################################
#################################################################
#################################################################

include $(LIBRARY_DIR)/this_dir.mk
include $(LIBRARY_DIR)/options.mk

#Mappings --------------
OBJECTS=$(patsubst %.cc,%.o, $(CCFILES))
GOBJECTS=$(patsubst %,.debug_objs/%, $(OBJECTS))

#Rules ------------------

%.o: %.cc $(HEADERS)
	$(CCCOM) -c $(CCFLAGS) -I$(eigenpath) -o $@ $<

.debug_objs/%.o: %.cc $(HEADERS)
	$(CCCOM) -c $(CCGFLAGS) -I$(eigenpath) -o $@ $<

#Targets -----------------

build: $(APP)
debug: $(APP)-g

$(APP): $(OBJECTS) $(ITENSOR_LIBS)
	$(CCCOM) $(CCFLAGS) $(OBJECTS) -o $(APP) $(LIBFLAGS)

$(APP)-g: mkdebugdir $(GOBJECTS) $(ITENSOR_GLIBS)
	$(CCCOM) $(CCGFLAGS) $(GOBJECTS) -o $(APP)-g $(LIBGFLAGS)

clean:
	rm -fr .debug_objs *.o $(APP) $(APP)-g

mkdebugdir:
	mkdir -p .debug_objs
