function [J grad] = nnCostFunction(nn_params, ...
                                   input_layer_size, ...
                                   hidden_layer_size, ...
                                   num_labels, ...
                                   X, y, lambda)

Theta1 = reshape(nn_params(1:hidden_layer_size * (input_layer_size + 1)), ...
                 hidden_layer_size, (input_layer_size + 1));

Theta2 = reshape(nn_params((1 + (hidden_layer_size * (input_layer_size + 1))):end), ...
                 num_labels, (hidden_layer_size + 1));

m = size(X, 1);

J = 0;
Theta1_grad = zeros(size(Theta1));
Theta2_grad = zeros(size(Theta2));


Y1 = zeros(m, num_labels);
for i = 1:m,
    Y1(i, y(i)) = 1;
end;

XP1 = [ones(m, 1), X];
Z2 = XP1 * Theta1';
A2 = sigmoid(Z2);
AP2 = [ones(m, 1), A2];
Z3 = AP2 * Theta2';
A3 = sigmoid(Z3);

LM = Y1 .* log(A3) + (1 - Y1) .* log(1 - A3);
J = sum(sum(LM)) / m * (-1);
Theta11 = Theta1(:, 2:size(Theta1, 2));
Theta22 = Theta2(:, 2:size(Theta2, 2));
J = J + lambda / (2*m) * (sum(sum(Theta11 .^ 2)) + sum(sum(Theta22 .^ 2)));

delta3 = A3 - Y1;
sigGrad = sigmoidGradient([zeros(size(Z2,1), 1) Z2]);
delta2 = delta3 * Theta2 .* sigGrad;
delta2 = delta2(:, 2:end);
Theta2_grad = delta3' * AP2;
Theta1_grad = delta2' * XP1;
Theta2_grad = Theta2_grad ./ m;
Theta1_grad = Theta1_grad ./ m;

Theta1_tmp=Theta1(:,[2:size(Theta1,2)]);
Theta2_tmp=Theta2(:,[2:size(Theta2,2)]);
Theta1_tmp=[zeros(size(Theta1,1),1) Theta1_tmp];
Theta2_tmp=[zeros(size(Theta2,1),1) Theta2_tmp];
Theta1_grad=Theta1_grad+lambda/m*Theta1_tmp;
Theta2_grad=Theta2_grad+lambda/m*Theta2_tmp;


% Unroll gradients
grad = [Theta1_grad(:) ; Theta2_grad(:)];


