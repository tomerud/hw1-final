import sys
import math

MAX_ITER = 200
EPSILON = 0.001


def k_means(k, d, max_iter, input_data):
    with open(input_data, "r") as input_file:
        data_points = [[float(x) for x in line.split(",")] for line in input_file.readlines()]
        centroids = data_points[:k]

    return calc_clusters(k, d, max_iter, data_points, centroids)


def calc_clusters(k, d, max_iter, data_points, centroids):
    for i in range(max_iter):
        old_centroids = centroids.copy()
        counts, sums = calc_closet_centroid(k, d, data_points, centroids)
        centroids = update_centroids(k, d, counts, sums)

        if centroids_converged(old_centroids, centroids, k, d):
            break

    for centroid in centroids:
        print(','.join(['%.4f' % c for c in centroid]))


def calc_closet_centroid(k, d, data_points, centroids):
    counts = [0 for _ in range(k)]  # how many data points are assigned to each centroid
    sums = [[0.0 for _ in range(d)] for _ in range(k)]  # sums of the distances

    for data_point in data_points:
        min_index = 0
        min_dist = float('inf')

        for i in range(k):  # find the index of the closest centroid
            dist = math.sqrt(sum((data_point[j] - centroids[i][j]) ** 2 for j in range(d)))
            if dist < min_dist:  # there is a closer centroid
                min_dist = dist
                min_index = i

        counts[min_index] += 1
        for m in range(len(data_point)):
            sums[min_index][m] += data_point[m]

    return counts, sums


def update_centroids(k, d, counts, sums):
    updated_centroids = [[0.0 for _ in range(d)] for _ in range(k)]

    for i in range(k):
        if counts[i] != 0:
            for j in range(d):
                updated_centroids[i][j] = sums[i][j] / counts[i]

    return updated_centroids


def centroids_converged(old_centroids, new_centroids, k, d):
    for j in range(k):
        dist = calc_distance(new_centroids[j], old_centroids[j], d)
        if dist >= EPSILON:
            return False
    return True


def calc_distance(p1, p2, d):
    # calculate Euclidean distance between two points
    sum = 0
    for i in range(d):
        sum += (math.pow((p1[i] - p2[i]), 2))
    return math.sqrt(sum)


def main(argv):
    if len(argv) < 5 or len(argv) > 6:
        print("An Error Has Occurred")
        sys.exit()

    if not 1 < int(argv[1]) < int(argv[2]) or not argv[1].isnumeric():
        print("Invalid number of clusters!")
        sys.exit()

    if not int(argv[2]) > 1 or not argv[2].isnumeric():
        print("Invalid number of points!")
        sys.exit()

    if not argv[3].isnumeric():
        print("Invalid dimension of points!")
        sys.exit()

    if len(argv) == 6:
        if not 1 < int(argv[4]) < 1000 or not argv[4].isnumeric():
            print("Invalid maximum iteration!")
            sys.exit()

        else:
            k = int(argv[1])
            d = int(argv[3])
            max_iter = int(argv[4])
            input_data = argv[5]
            return k_means(k, d, max_iter, input_data)

    if len(argv) == 5:
        k = int(argv[1])
        d = int(argv[3])
        max_iter = MAX_ITER
        input_data = argv[4]
        return k_means(k, d, max_iter, input_data)


if __name__ == '__main__':
    main(sys.argv)
