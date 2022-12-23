#include <cmath>
#include <numeric>
#include <algorithm>
#include <gtest/gtest.h>
#include <inner_carving.h>





class DistanceFromPlaneTest : public ::testing::Test {
protected:
    void SetUp(const Eigen::Vector3d & query) {
      distance = distance_from_plane(query, Eigen::Vector3d(1, 1, 0), Eigen::Vector3d(2, 2, 0));
    }

    double distance {};
};

TEST_F(DistanceFromPlaneTest, OnThePlane) {
  SetUp(Eigen::Vector3d(0.5, 1.5, 10));
  EXPECT_DOUBLE_EQ(distance, 0);
}


TEST_F(DistanceFromPlaneTest, PositiveDistance) {
  SetUp(Eigen::Vector3d(2, 4, 10));
  EXPECT_DOUBLE_EQ(distance, 4);
}


TEST_F(DistanceFromPlaneTest, NegativeDistance) {
  SetUp(Eigen::Vector3d(-2, 0, -10));
  EXPECT_DOUBLE_EQ(distance, -4);
}


class SortComparatorTest : public ::testing::Test {
protected:
    void SetUp() override {
      contact << 1, 1, 0;
      CoM << 2, 2, 0;

      queries.resize(6, 3);
      queries << 1, 2, 0,
                 2, 2, 1,
                 2, 1, 2,
                 0, 1, 3,
                 1, 0, 4,
                 0, 0, 5;

      comp = [&](int i, int j)
      {
          return distance_from_plane(queries.row(i), contact, CoM)
                 > distance_from_plane(queries.row(j), contact, CoM);
      };

      indices.resize(6);
      std::iota(indices.begin(), indices.end(), 0);
    }

    Eigen::Vector3d contact;
    Eigen::Vector3d CoM;
    Eigen::MatrixXd queries;
    std::function<bool (int, int)> comp;
    std::vector<int> indices;
};

// test for both in front of and behind plane and one in front and one behind
TEST_F(SortComparatorTest, SameDistance) {
  // both positive distance
  ASSERT_FALSE(comp(0, 2));
  EXPECT_FALSE(comp(2, 0));

  // both negative distance
  ASSERT_FALSE(comp(3, 4));
  EXPECT_FALSE(comp(4, 3));
}

TEST_F(SortComparatorTest, FirstGreaterThanSecond) {
  // both positive distance
  EXPECT_TRUE(comp(1, 0));

  // both negative distance
  EXPECT_TRUE(comp(4, 5));

  // one positive and one negative
  EXPECT_TRUE(comp(2, 4));
}

TEST_F(SortComparatorTest, SecondGreaterThanFirst) {
  // both positive distance
  EXPECT_FALSE(comp(2, 1));

  // both negative distance
  EXPECT_FALSE(comp(5, 3));

  // one positive and one negative
  EXPECT_FALSE(comp(5, 1));
}

TEST_F(SortComparatorTest, Sort) {
  std::sort(indices.begin(), indices.end(), comp);

  ASSERT_TRUE(indices[0] == 1);
  ASSERT_TRUE(indices[1] == 0 || indices[1] == 2);
  ASSERT_TRUE(indices[2] == 0 || indices[2] == 2);
  ASSERT_TRUE(indices[3] == 3 || indices[3] == 4);
  ASSERT_TRUE(indices[4] == 3 || indices[4] == 4);
  ASSERT_TRUE(indices[5] == 5);
}


TEST(BuildCarvedTest, Empty) {
  std::vector<int> indices;
  int mask[5];
  build_carved(indices.begin(), indices.end(), 5, mask);
  for (int i : mask) {
    EXPECT_TRUE(mask[i] == 1);
  }
}


TEST(BuildCarvedTest, FirstAndLast) {
  std::vector<int> indices;
  indices.emplace_back(0);
  indices.emplace_back(4);
  int mask[5];
  build_carved(indices.begin(), indices.end(), 5, mask);

  EXPECT_TRUE(mask[0] == -1);
  EXPECT_TRUE(mask[4] == -1);

  for (int i = 1; i < 4; ++i) {
    EXPECT_TRUE(mask[i] == 1);
  }
}


TEST(BuildCarvedTest, Arbitrary) {
  std::vector<int> indices;
  indices.emplace_back(5);
  indices.emplace_back(7);
  indices.emplace_back(2);
  indices.emplace_back(3);
  indices.emplace_back(6);
  int mask[10];
  build_carved(indices.begin(), indices.end(), 10, mask);

  EXPECT_TRUE(mask[0] == 1);
  EXPECT_TRUE(mask[1] == 1);
  EXPECT_TRUE(mask[2] == -1);
  EXPECT_TRUE(mask[3] == -1);
  EXPECT_TRUE(mask[4] == 1);
  EXPECT_TRUE(mask[5] == -1);
  EXPECT_TRUE(mask[6] == -1);
  EXPECT_TRUE(mask[7] == -1);
  EXPECT_TRUE(mask[8] == 1);
  EXPECT_TRUE(mask[9] == 1);
}