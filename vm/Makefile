all: assemble.me conditionals.me
assemble.me: assemble.me.tpl
	./prepare.sh assemble.me
conditionals.me: conditionals.me.tpl
	./prepare.sh conditionals.me
common: common-cog.h
	python -m cogapp common-cog.h > common.h
