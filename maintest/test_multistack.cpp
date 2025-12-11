#include <gtest.h>
#include <MultiStack.h>

TEST(TMultiStackTest, DefaultConstructor)
{
TMultiStack<int> stack;
EXPECT_EQ(stack.GetStacksCount(), 0);
EXPECT_EQ(stack.GetTotalCapacity(), 0);
EXPECT_EQ(stack.GetMemory(), nullptr);
EXPECT_EQ(stack.GetTops(), nullptr);
EXPECT_EQ(stack.GetCapacities(), nullptr);
}

TEST(TMultiStackTest, ParameterizedConstructor)
{
TMultiStack<int> stack(3, 100);
EXPECT_EQ(stack.GetStacksCount(), 3);
EXPECT_EQ(stack.GetTotalCapacity(), 100);
EXPECT_NE(stack.GetMemory(), nullptr);
EXPECT_NE(stack.GetTops(), nullptr);
EXPECT_NE(stack.GetCapacities(), nullptr);
}

TEST(TMultiStackTest, PushPopOperations)
{
TMultiStack<int> stack(2, 10);

stack.push(0, 42);
stack.push(1, 77);

EXPECT_FALSE(stack.IsEmpty(0));
EXPECT_FALSE(stack.IsEmpty(1));
EXPECT_EQ(stack.pop(0), 42);
EXPECT_EQ(stack.pop(1), 77);
EXPECT_TRUE(stack.IsEmpty(0));
EXPECT_TRUE(stack.IsEmpty(1));
}

TEST(TMultiStackTest, StackSize)
{
TMultiStack<int> stack(2, 10);

stack.push(0, 1);
stack.push(0, 2);
stack.push(1, 3);

EXPECT_EQ(stack.Size(0), 2);
EXPECT_EQ(stack.Size(1), 1);
}

TEST(TMultiStackTest, OverflowBehavior)
{
TMultiStack<int> stack(2, 2);

stack.push(0, 1);
stack.push(0, 2);

EXPECT_TRUE(stack.IsFull(0));

stack.push(0, 3);

EXPECT_EQ(stack.Size(0), 3);
}

TEST(TMultiStackTest, EqualityOperator)
{
TMultiStack<int> stack1(2, 10);
TMultiStack<int> stack2(2, 10);

stack1.push(0, 5);
stack2.push(0, 5);

EXPECT_TRUE(stack1 == stack2);

stack2.push(0, 6);
EXPECT_FALSE(stack1 == stack2);
}

TEST(TMultiStackTest, InequalityOperator)
{
TMultiStack<int> stack1(2, 10);
TMultiStack<int> stack2(3, 10);

EXPECT_TRUE(stack1 != stack2);
}

TEST(TMultiStackTest, IteratorTraversal)
{
TMultiStack<int> stack(1, 5);
stack.push(0, 1);
stack.push(0, 2);
stack.push(0, 3);

auto it = stack.begin(0);
auto end = stack.end(0);

EXPECT_EQ(*it, 1);
++it;
EXPECT_EQ(*it, 2);
it++;
EXPECT_EQ(*it, 3);
++it;
EXPECT_EQ(it, end);
}

TEST(TMultiStackTest, EmptyIterator)
{
TMultiStack<int> stack(1, 5);
auto it = stack.begin(0);
auto end = stack.end(0);

EXPECT_EQ(it, end);
}

TEST(TMultiStackTest, MoveOperations)
{
TMultiStack<int> stack1(2, 10);
stack1.push(0, 42);

TMultiStack<int> stack2(std::move(stack1));
EXPECT_EQ(stack2.pop(0), 42);
EXPECT_EQ(stack2.GetStacksCount(), 2);
}

TEST(TMultiStackTest, CopyOperations)
{
TMultiStack<int> stack1(2, 10);
stack1.push(0, 7);

TMultiStack<int> stack2(stack1);
EXPECT_EQ(stack2.pop(0), 7);
EXPECT_EQ(stack1.Size(0), 1);
}
