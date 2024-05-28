#include <stdio.h>
#include <stdlib.h>

// 定义一个结构体来存储各种组合的价值
typedef struct {
    double values[128];  // 存储各种组合的价值，最多支持7个参与者的组合
} GameValue;

// 函数声明
double factorial(int n); // 计算阶乘的函数
double shapley_value(int player, int total_players, GameValue *gameValue);

int main() {
    int total_players = 0;
    printf("请输入参与者总数: ");
    scanf("%d", &total_players);

    // 确保参与者数在合理范围内
    if (total_players <= 0 || total_players > 7) {
        printf("参与者总数必须在1到7之间\n");
        return 1;
    }

    GameValue gameValue = {0};  // 初始化所有价值为0

    printf("请输入每个参与者组合的价值:\n");
    int max_subset = (1 << total_players) - 1;
    for (int i = 1; i <= max_subset; i++) {
        printf("组合 [ ");
        for (int j = 0; j < total_players; j++) {
            if (i & (1 << j)) {
                printf("%d ", j + 1);
            }
        }
        printf("] 的组合值为: ");
        scanf("%lf", &gameValue.values[i]);
    }

    printf("参与者1 到 %d的 Shapley 值:\n", total_players);
    for (int i = 1; i <= total_players; i++) {
        printf("参与者 %d: %.2f\n", i, shapley_value(i, total_players, &gameValue));
    }

    return 0;
}

// 计算阶乘的函数
double factorial(int n) {
    if (n == 0) return 1.0;
    double result = 1.0;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

// 计算指定参与者的Shapley值的函数
double shapley_value(int player, int total_players, GameValue *gameValue) {
    double shapley = 0.0;
    // 生成一个只包含指定参与者的子集掩码
    int subset_mask = 1 << (player - 1);
    // 遍历所有可能的子集
    for (int subset = 0; subset < (1 << total_players); subset++) {
        if ((subset & subset_mask) == 0) { // 检查子集是否包含指定参与者subset
            // 如果子集不包含指定参与者，则计算包含指定参与者的子集
            int subset_union_player = subset | subset_mask;
            int s = 0;  // 子集中的参与者数量
            for (int i = 0; i < total_players; i++) {
                if (subset & (1 << i)) s++;
            }
            double weight = (factorial(s) * factorial(total_players - s - 1)) / factorial(total_players);
            shapley += weight * (gameValue->values[subset_union_player] - gameValue->values[subset]);
        }
    }
    return shapley;
}
