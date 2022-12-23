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


// create a function that takes in two indices of queries, and return whether
// the first point into queries is closer to the plane than the second
// initialize queries with some interesting points, each pair represent a test case
//
class GenerateCompTest : public ::testing::Test {
protected:
    void SetUp() override {
      queries.resize(6, 3);
      queries << 1, 2, 0,
                 2, 2, 1,
                 2, 1, 2,
                 0, 1, 3,
                 1, 0, 4,
                 0, 0, 5;

      Eigen::Vector3d contact(1, 1, 0);
      Eigen::Vector3d CoM(2, 2, 0);
      comp = generate_comp(queries, contact, CoM);
    }

    Eigen::MatrixXd queries;
    std::function<bool (int, int)> comp;
};

// test for both in front of and behind plane and one in front and one behind
TEST_F(GenerateCompTest, SameDistance) {
  ASSERT_FALSE(comp(0, 2));
  EXPECT_FALSE(comp(2, 0));

  ASSERT_FALSE(comp(3, 4));
  EXPECT_FALSE(comp(4, 3));
}

TEST_F(GenerateCompTest, FirstGreaterThanSecond) {
  // both in front
  EXPECT_TRUE(comp(1, 0));

  // both behind
  EXPECT_TRUE(comp(4, 5));

  // one in the front, one behind
  EXPECT_TRUE(comp(2, 3));
}

TEST_F(GenerateCompTest, SecondGreaterThanFirst) {
  // both in front
  EXPECT_TRUE(comp(1, 2));

  // both behind
  EXPECT_TRUE(comp(5, 3));

  // one in the front, one behind
  EXPECT_TRUE(comp(5, 1));
}


// Create a lambda expression that  takes in two indices of queries, and return whether
// the first point into queries is closer to the plane than the second
// initialize queries with some interesting points, each pair represent a test case
//
class GenerateCompTestBeta : public ::testing::Test {
protected:
    void SetUp() override {
      queries.resize(6, 3);
      queries << 1, 2, 0,
                2, 2, 1,
                2, 1, 2,
                0, 1, 3,
                1, 0, 4,
                0, 0, 5;

      comp = generate_comp(queries, Eigen::Vector3d(1, 1, 0), Eigen::Vector3d(2, 2, 0));
    }

    Eigen::MatrixXd queries;
    std::function<bool (int, int)> comp;
};


TEST(name, name) {
  Eigen::Vector3d contact = Eigen::Vector3d(1, 1, 0);
  Eigen::Vector3d CoM = Eigen::Vector3d(2, 2, 0);
  Eigen::MatrixXd grid(6, 3);
  grid << 1, 2, 0,
    2, 2, 1,
    2, 1, 2,
    0, 1, 3,
    1, 0, 4,
    0, 0, 5;

  std::function<bool (int, int)> comp = [&](int i, int j)
  {
      double d1 = distance_from_plane(grid.row(i), contact, CoM);
      double d2 = distance_from_plane(grid.row(j), contact, CoM);
      return d1 > d2;
  };

  ASSERT_FALSE(comp(0, 2));
  EXPECT_FALSE(comp(2, 0));

  ASSERT_FALSE(comp(3, 4));
  EXPECT_FALSE(comp(4, 3));

//  comp(4, 5);
//  comp(0, 1);
//  comp(2, 4);
  // both in front
  EXPECT_TRUE(comp(1, 0));

  // both behind
  EXPECT_TRUE(comp(4, 5));

  // one in the front, one behind
  EXPECT_TRUE(comp(2, 4));


  EXPECT_FALSE(comp(2, 1));

  // both behind
  EXPECT_FALSE(comp(5, 3));

  // one in the front, one behind
  EXPECT_FALSE(comp(5, 1));

  std::vector<int>indices(6);
  std::iota(indices.begin(), indices.end(), 0);
  std::sort(indices.begin(), indices.end(), comp);

  ASSERT_TRUE(indices[0] == 1);
  ASSERT_TRUE(indices[1] == 0 || indices[1] == 2);
  ASSERT_TRUE(indices[2] == 0 || indices[2] == 2);
  ASSERT_TRUE(indices[3] == 3 || indices[3] == 4);
  ASSERT_TRUE(indices[4] == 3 || indices[4] == 4);
  ASSERT_TRUE(indices[5] == 5);
}


// test for both in front of and behind plane and one in front and one behind
TEST_F(GenerateCompTestBeta, SameDistance) {
  ASSERT_FALSE(comp(0, 2));
  EXPECT_FALSE(comp(2, 0));

  ASSERT_FALSE(comp(3, 4));
  EXPECT_FALSE(comp(4, 3));
}

TEST_F(GenerateCompTestBeta, FirstCloser) {
  // both in front
  EXPECT_FALSE(comp(0, 1));

  // both behind
  EXPECT_FALSE(comp(4, 5));

  // one in the front, one behind
  EXPECT_FALSE(comp(2, 4));
}

TEST_F(GenerateCompTestBeta, SecondCloser) {
  // both in front
  EXPECT_TRUE(comp(1, 2));

  // both behind
  EXPECT_TRUE(comp(5, 3));

  // one in the front, one behind
  EXPECT_TRUE(comp(5, 1));
}


TEST(BuildInOutTest, Empty) {
  std::vector<int> indices;
  int in_out[5];
  build_in_out(indices.begin(), indices.end(), 5, in_out);
  for (int i : in_out) {
    EXPECT_TRUE(in_out[i] == 1);
  }
}


TEST(BuildInOutTest, FirstAndLast) {
  std::vector<int> indices;
  indices.emplace_back(0);
  indices.emplace_back(4);
  int in_out[5];
  build_in_out(indices.begin(), indices.end(), 5, in_out);

  EXPECT_TRUE(in_out[0] == -1);
  EXPECT_TRUE(in_out[4] == -1);

  for (int i = 1; i < 4; ++i) {
    EXPECT_TRUE(in_out[i] == 1);
  }
}


TEST(BuildInOutTest, Arbitrary) {
  std::vector<int> indices;
  indices.emplace_back(5);
  indices.emplace_back(7);
  indices.emplace_back(2);
  indices.emplace_back(3);
  indices.emplace_back(6);
  int in_out[10];
  build_in_out(indices.begin(), indices.end(), 10, in_out);

  EXPECT_TRUE(in_out[0] == 1);
  EXPECT_TRUE(in_out[1] == 1);
  EXPECT_TRUE(in_out[2] == -1);
  EXPECT_TRUE(in_out[3] == -1);
  EXPECT_TRUE(in_out[4] == 1);
  EXPECT_TRUE(in_out[5] == -1);
  EXPECT_TRUE(in_out[6] == -1);
  EXPECT_TRUE(in_out[7] == -1);
  EXPECT_TRUE(in_out[8] == 1);
  EXPECT_TRUE(in_out[9] == 1);
}