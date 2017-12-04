BUILD_DIR=build

cbuild: prepare
	$(MAKE) -C $(BUILD_DIR) all

verbose: prepare
	$(MAKE) -C $(BUILD_DIR) VERBOSE=1 all

test: cbuild
	$(MAKE) -C $(BUILD_DIR) test

vtest: cbuild
	$(MAKE) -C $(BUILD_DIR) test ARGS="-VV"

install: prepare
	$(MAKE) -C $(BUILD_DIR) install

uninstall:
	$(MAKE) -C $(BUILD_DIR) uninstall

deb rpm package: prepare
	cd $(BUILD_DIR) && cpack
	$(MAKE) -C $(BUILD_DIR) package-fix

prepare:
	if [ ! -d $(BUILD_DIR) ]; then mkdir $(BUILD_DIR); fi
	if [ ! -f $(BUILD_DIR)/Makefile ]; then cd $(BUILD_DIR) && cmake ..; fi

tags::
	rm -f $@
	ctags -R --exclude="cscope.*" --exclude=$(BUILD_DIR)/rpmbuild/* .

cscope:
	rm -f cscope.*
	find . -name '*.[ch]' -o -name '*.cc' -o -name '*.cpp' -o -name '*.hpp' | grep -v "$(BUILD_DIR)/" > cscope.files
	cscope -kbi cscope.files

index: tags cscope

clean:
	if [ -d $(BUILD_DIR) ]; then $(MAKE) -C $(BUILD_DIR) clean; fi

mrproper:
	rm -f cscope.* tags
	rm -rf $(BUILD_DIR)/
