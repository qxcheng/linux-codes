load ('ex7data1.mat');

[X_norm, mu, sigma] = featureNormalize(X);

[U, S] = pca(X_norm);

K = 1;

%投影数据
Z = projectData(X_norm, U, K);

%恢复数据
X_rec  = recoverData(Z, U, K);

