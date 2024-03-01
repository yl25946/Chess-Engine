dogs = ["hi", "man", "tulip"]
favNumbers = [3, 10, 5940, 8132]


def dogNums(dogs, numbers):
    for dog in dogs:
        for num in numbers:
            print(dog, num)


dogNums(dogs, favNumbers)
print(len(dogs))

# def mean(nums):
favNumbers = favNumbers + [75, 8]
dogNums(dogs, favNumbers)
