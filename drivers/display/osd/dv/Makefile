.PHONY : dovi.o
dovi.o:
	@echo "start to make dovi.o"
	mkdir -p $(buildtree)/drivers/display/osd/dv
	cp -a $(buildsrc)/drivers/display/osd/dv/dovi.a \
		$(buildtree)/drivers/display/osd/dv/dovi.o

obj-$(CONFIG_AML_DOLBY) += dovi.o
