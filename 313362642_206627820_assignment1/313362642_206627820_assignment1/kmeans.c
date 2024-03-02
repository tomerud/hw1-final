#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITER 200
#define EPSILON 0.001

double distance(double *p1, double *p2, int d) {
    double sum = 0.0;
    int i;
    for (i = 0; i < d; i++) {
        sum += (p1[i] - p2[i]) * (p1[i] - p2[i]);
    }
    return sqrt(sum);
}

void initialize_centroids(double **centroids, double **data, int k, int d) {
    int i, j;
    for (i = 0; i < k; i++) {
        for (j = 0; j < d; j++) {
            centroids[i][j] = data[i][j];
        }
    }
}

void assign_clusters(int *clusters, double **data, double **centroids, int k, int n, int d) {
    int i, j;
    for (i = 0; i < n; i++) {
        double min_dist = 10000; 
        int min_index = -1;
        for (j = 0; j < k; j++) {
            double dist = distance(data[i], centroids[j], d);
            if (dist < min_dist) {
                min_dist = dist;
                min_index = j;
            }
        }
        clusters[i] = min_index;
    }
}

void update_centroids(double **centroids, double **data, int *clusters, int k, int n, int d) {
    int i, j, l, count;
    double *sums;
    for (i = 0; i < k; i++) {
        sums = (double *)calloc(d, sizeof(double));
        if (!sums) { 
            printf("An Error Has Occurred\n");
            exit(1); 
        }
        count = 0;
        for (j = 0; j < n; j++) {
            if (clusters[j] == i) {
                count++;
                for (l = 0; l < d; l++) {
                    sums[l] += data[j][l];
                }
            }
        }
        for (l = 0; l < d; l++) {
            centroids[i][l] = sums[l] / count;
        }
        free(sums);
    }
}

int centroids_converged(double **centroids, double **old_centroids, int k, int d, double epsilon) {
    int i;
    for (i = 0; i < k; i++) {
        if (distance(centroids[i], old_centroids[i], d) >= epsilon) {
            return 0;
        }
    }
    return 1;
}

void copy_centroids(double **source, double **destination, int k, int d) {
    int i, j;
    for (i = 0; i < k; i++) {
        for (j = 0; j < d; j++) {
            destination[i][j] = source[i][j];
        }
    }
}

int main(int argc, char *argv[]) {
    char *end;
    long N, K, d, iter;
    double **data, **centroids, **old_centroids;
    int *clusters;
    int i, j;

    if (argc < 4 || argc > 5) {
        printf("An Error Has Occurred\n");
        return 1;
    }

    N = strtol(argv[2], &end, 10);
    if (*end != '\0' || N <= 1) {
        printf("Invalid number of points!\n");
        return 1;
    }

    K = strtol(argv[1], &end, 10);
    if (*end != '\0' || K <= 1 || K >= N) {
        printf("Invalid number of clusters!\n");
        return 1;
    }

    d = strtol(argv[3], &end, 10);
    if (*end != '\0' || d < 1) {
        printf("Invalid dimension of point!\n");
        return 1;
    }

    iter = MAX_ITER;

    if (argc > 4) {
        iter = strtol(argv[4], &end, 10);
        if (*end != '\0' || iter <= 1 || iter >= 1000) {
            printf("Invalid maximum iteration!\n");
            return 1;
        }
    }

    data = (double **)malloc(N * sizeof(double *));
    if (!data) {
        printf("An Error Has Occurred\n");
        return 1;
    }
    for (i = 0; i < N; i++) {
        data[i] = (double *)malloc(d * sizeof(double));
        if (!data[i]) {
            printf("An Error Has Occurred\n");
            return 1;
        }
        for (j = 0; j < d; j++) {
            scanf("%lf,", &data[i][j]);
        }
    }

    centroids = (double **)malloc(K * sizeof(double *));
    if (!centroids) {
        printf("An Error Has Occurred\n");
        return 1;
    }
    for (i = 0; i < K; i++) {
        centroids[i] = (double *)malloc(d * sizeof(double));
        if (!centroids[i]) {
            printf("An Error Has Occurred\n");
            return 1;
        }
    }

    old_centroids = (double **)malloc(K * sizeof(double *));
    if (!old_centroids) {
        printf("An Error Has Occurred\n");
        return 1;
    }
    for (i = 0; i < K; i++) {
        old_centroids[i] = (double *)malloc(d * sizeof(double));
        if (!old_centroids[i]) {
            printf("An Error Has Occurred\n");
            return 1;
        }
    }

    initialize_centroids(centroids, data, K, d);
    initialize_centroids(old_centroids, data, K, d);

    clusters = (int *)malloc(N * sizeof(int));
    if (!clusters) {
        printf("An Error Has Occurred\n");
        return 1;
    }

    for (i = 0; i < iter; i++) {
        assign_clusters(clusters, data, centroids, K, N, d);
        update_centroids(centroids, data, clusters, K, N, d);
        if (centroids_converged(centroids, old_centroids, K, d, EPSILON)) {
            break;
        }
        copy_centroids(centroids, old_centroids, K, d);
    }

    for (i = 0; i < K; i++) {
        for (j = 0; j < d; j++) {
            printf("%.4f ", centroids[i][j]);
        }
        printf("\n");
    }

    for (i = 0; i < N; i++) {
        free(data[i]);
    }
    free(data);

    for (i = 0; i < K; i++) {
        free(centroids[i]);
        free(old_centroids[i]);
    }
    free(centroids);
    free(old_centroids);

    free(clusters);

    return 0;
}