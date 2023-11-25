for n in [10, 100, 500,1000, 1500, 2000, 2500, 3000]:
    sum = 0
    for i in range(2, n + 1):
        sum += 3 * i + 2 * i**2
    print(sum)
