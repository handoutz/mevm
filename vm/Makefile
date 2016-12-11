assemble.me: assemble.me.tpl
	./prepare.sh
common: common-cog.h
	python -m cogapp common-cog.h > common.h
all: assemble.me common
