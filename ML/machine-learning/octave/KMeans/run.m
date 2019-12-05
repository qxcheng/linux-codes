A = double(imread('bird_small.png'));
A = A / 255;

img_size = size(A);
X = reshape(A, img_size(1) * img_size(2), 3);

K = 16; 
max_iters = 10;

initial_centroids = kMeansInitCentroids(X, K);

[centroids, idx] = runkMeans(X, initial_centroids, max_iters);

idx = findClosestCentroids(X, centroids);
X_recovered = centroids(idx,:);
X_recovered = reshape(X_recovered, img_size(1), img_size(2), 3);

% 未压缩前的图片
subplot(1, 2, 1);
imagesc(A); 
title('Before Compression');

% 压缩后的图片
subplot(1, 2, 2);
imagesc(X_recovered)
title('After Compression');