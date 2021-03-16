#include <stdio.h>
#include <stdlib.h>
#include <check.h>
#include "malloc.h"

START_TEST(allocated_page)
{
    ck_assert_uint_ge(size_of_page(0), 0);
    ck_assert_uint_ge(size_of_page(10), 10);
    ck_assert_uint_ge(size_of_page(4096), 4096);
    ck_assert_uint_ge(size_of_page(8192), 8192);
}
END_TEST

Suite *unit_test(void)
{
    Suite *suite = suite_create("\n=========UNIT TESTS=========");

    TCase *page_allocated = tcase_create("\nTest of allocated page");

    tcase_add_test(page_allocated, allocated_page);

    suite_add_tcase(suite, page_allocated);

    return suite;
}

int main(void)
{
    Suite *suite = unit_test();
    SRunner *srun = srunner_create(suite);

    int rest;
    srunner_run_all(srun, CK_VERBOSE);
    rest = srunner_ntests_failed(srun);
    srunner_free(srun);
    printf("\n\n");
    return (rest == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}