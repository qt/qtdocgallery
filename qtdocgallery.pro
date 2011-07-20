TEMPLATE = subdirs
CONFIG += ordered

module_qtdocgallery_src.subdir = src
module_qtdocgallery_src.target = module-qtdocgallery-src

#module_qtdocgallery_tests.subdir = tests
#module_qtdocgallery_tests.target = module-qtdocgallery-tests
#module_qtdocgallery_tests.depends = module_qtdocgallery_src
#!contains(QT_BUILD_PARTS,tests) {
#    module_qtdocgallery_tests.CONFIG = no_default_target no_default_install
#}

SUBDIRS += module_qtdocgallery_src \
#           module_qtdocgallery_tests
