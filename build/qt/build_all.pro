TEMPLATE = subdirs

CONFIG += debug_and_release
#QMAKE_MAC_SDK = macosx10.12

SUBDIRS += \
    RUC1 \
    RUC2 \
	libiconv \
	libxml2 \
	ATCore \
	ATProject \
	ATLinker \
	ATGUI \
	EDFDEditor \
	UCore \
	EDFDCover \
	ATPlanner \
	DSFEditor \
	ATTextEditor \
	KBEditor \
        ATVerifier \
	KMPZ \
	ATWorkbench \
        KnowledgeField \
        DataMining \
        Merger \
        Protocols \
        Tests/EDFDCoverTest
