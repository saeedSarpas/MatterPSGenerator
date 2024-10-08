#include <cgreen/cgreen.h>
#include <math.h>

#include "./../../global_structs/config_struct.h"

#include "./../../global_functions/io/open_file.h"

#include "./my_libconfig.h"
#include "./load_config_from.h"

#define FILE_INDEX 1
#define MASS_ASSIGNMENT_INDEX 1
#define BINNING_INDEX 1
#define NUM_OF_AXIS_GRIDS 1
#define ALIAS "alias"
#define FILENAME "filename"
#define MASS_ASSIGNMENT_NAME "mass_assignment_name"
#define BINNING_NAME "binning_name"
#define ALGORITHM_NAME "algorithm_name"
#define CONF_FILE_ADDR "./conf.test"

void write_new_config_file ();

Describe (load_config_from);

BeforeEach (load_config_from) {
  write_new_config_file();
}

AfterEach (load_config_from) {
	remove(CONF_FILE_ADDR);
}

Ensure (load_config_from, fills_config_struct_with_right_configs) {

	config_struct conf = load_config_from(CONF_FILE_ADDR);

	int i;
	for (i = 0; i < 2; i++) {
		assert_that(conf.files[i].filename, is_equal_to_string(FILENAME));
		assert_that(conf.files[i].alias, is_equal_to_string(ALIAS));

		assert_that(conf.massFunctions[i].name,
					is_equal_to_string(MASS_ASSIGNMENT_NAME));
		assert_that(conf.massFunctions[i].alias,
					is_equal_to_string(ALIAS));

		assert_that(conf.binning[i].name, is_equal_to_string(BINNING_NAME));
		assert_that(conf.binning[i].alias, is_equal_to_string(ALIAS));
	}

	assert_that(conf.params.fileIndex, is_equal_to(FILE_INDEX));
	assert_that(conf.params.massAssignmentIndex,
				is_equal_to(MASS_ASSIGNMENT_INDEX));
	assert_that(conf.params.binningIndex, is_equal_to(BINNING_INDEX));
	assert_that(conf.params.numOfAxisGrids,
				is_equal_to(NUM_OF_AXIS_GRIDS));
}

TestSuite *load_config_from_tests() {
	TestSuite *suite = create_test_suite();
	add_test_with_context(suite, load_config_from,
						 fills_config_struct_with_right_configs);
	return suite;
}

void write_new_config_file () {
	int i;

	config_t cfg;
	config_setting_t *root, *run_params_node, *first_node, *second_node,
		*groups, *setting;

	libconfig_init(&cfg);
	root = config_root_setting(&cfg);

	run_params_node = libconfig_setting_add(root, "run_params", CONFIG_TYPE_GROUP);

	setting = libconfig_setting_add(run_params_node, "file_index", CONFIG_TYPE_INT);
	libconfig_setting_set_int(setting, FILE_INDEX);

	setting = libconfig_setting_add(run_params_node, "mass_assignment_index", CONFIG_TYPE_INT);
	libconfig_setting_set_int(setting, MASS_ASSIGNMENT_INDEX);

	setting = libconfig_setting_add(run_params_node, "binning_index", CONFIG_TYPE_INT);
	libconfig_setting_set_int(setting, BINNING_INDEX);

	setting = libconfig_setting_add(run_params_node, "num_of_axis_grids", CONFIG_TYPE_INT);
	libconfig_setting_set_int(setting, NUM_OF_AXIS_GRIDS);

	first_node = libconfig_setting_add(root, "input", CONFIG_TYPE_GROUP);
	second_node = libconfig_setting_add(first_node, "files", CONFIG_TYPE_LIST);
	for (i = 0; i < 2; i++) {
		groups = libconfig_setting_add(second_node, NULL, CONFIG_TYPE_GROUP);

		setting = libconfig_setting_add(groups, "index", CONFIG_TYPE_INT);
		libconfig_setting_set_int(setting, i);

		setting = libconfig_setting_add(groups, "filename", CONFIG_TYPE_STRING);
		libconfig_setting_set_string(setting, FILENAME);

		setting = libconfig_setting_add(groups, "alias", CONFIG_TYPE_STRING);
		libconfig_setting_set_string(setting, ALIAS);
	}

	first_node = libconfig_setting_add(root, "griding", CONFIG_TYPE_GROUP);
	second_node = libconfig_setting_add(first_node, "mass_assignment_functions", CONFIG_TYPE_LIST);
	for (i = 0; i < 2; i++) {
		groups = libconfig_setting_add(second_node, NULL, CONFIG_TYPE_GROUP);

		setting = libconfig_setting_add(groups, "index", CONFIG_TYPE_INT);
		libconfig_setting_set_int(setting, i);

		setting = libconfig_setting_add(groups, "name", CONFIG_TYPE_STRING);
		libconfig_setting_set_string(setting, MASS_ASSIGNMENT_NAME);

		setting = libconfig_setting_add(groups, "alias", CONFIG_TYPE_STRING);
		libconfig_setting_set_string(setting, ALIAS);
	}

	first_node = libconfig_setting_add(root, "binning", CONFIG_TYPE_GROUP);
	second_node = libconfig_setting_add(first_node, "algorithms", CONFIG_TYPE_LIST);
	for (i = 0; i < 2; i++) {
		groups = libconfig_setting_add(second_node, NULL, CONFIG_TYPE_GROUP);

		setting = libconfig_setting_add(groups, "index", CONFIG_TYPE_INT);
		libconfig_setting_set_int(setting, i);

		setting = libconfig_setting_add(groups, "name", CONFIG_TYPE_STRING);
		libconfig_setting_set_string(setting, BINNING_NAME);

		setting = libconfig_setting_add(groups, "alias", CONFIG_TYPE_STRING);
		libconfig_setting_set_string(setting, ALIAS);
	}

	FILE *conf_file;
	open_file(&conf_file, CONF_FILE_ADDR, "w");

	config_write(&cfg, conf_file);

	fclose(conf_file);
	libconfig_destroy(&cfg);
}
