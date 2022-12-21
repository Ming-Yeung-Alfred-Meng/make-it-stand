#include <gtest/gtest.h>
#include <linear_index.h>


TEST(LinearIndexTest, ArbitaryPointInArbitraryCuboid) {
  EXPECT_EQ(linear_index(Eigen::RowVector3i(8, 7, 10), 2, 4, 1), 90);
}

TEST(LinearIndexTest, CornerPointInArbitraryCuboid1) {
  EXPECT_EQ(linear_index(Eigen::RowVector3i(8, 7, 10), 0, 0, 0), 0);
}

TEST(LinearIndexTest, CornerPointInArbitraryCuboid2) {
  EXPECT_EQ(linear_index(Eigen::RowVector3i(8, 7, 10), 7, 0, 0), 7);
}

TEST(LinearIndexTest, CornerPointInArbitraryCuboid3) {
  EXPECT_EQ(linear_index(Eigen::RowVector3i(8, 7, 10), 0, 6, 0), 48);
}

TEST(LinearIndexTest, CornerPointInArbitraryCuboid4) {
  EXPECT_EQ(linear_index(Eigen::RowVector3i(8, 7, 10), 7, 6, 0), 55);
}

TEST(LinearIndexTest, CornerPointInArbitraryCuboid5) {
  EXPECT_EQ(linear_index(Eigen::RowVector3i(8, 7, 10), 0, 0, 9), 504);
}

TEST(LinearIndexTest, CornerPointInArbitraryCuboid6) {
  EXPECT_EQ(linear_index(Eigen::RowVector3i(8, 7, 10), 7, 0, 9), 511);
}

TEST(LinearIndexTest, CornerPointInArbitraryCuboid7) {
  EXPECT_EQ(linear_index(Eigen::RowVector3i(8, 7, 10), 0, 6, 9), 552);
}

TEST(LinearIndexTest, CornerPointInArbitraryCuboid8) {
  EXPECT_EQ(linear_index(Eigen::RowVector3i(8, 7, 10), 7, 6, 9), 559);
}