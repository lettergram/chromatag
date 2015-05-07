SUBDIRS = apriltags
CLEANDIRS = $(SUBDIRS:%=clean-%)

.PHONY: subdirs $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:	$(CLEANDIRS) 

$(CLEANDIRS): 
	$(MAKE) -C $(@:clean-%=%) clean
	@rm -rf *.o *~ apriltag_demo


