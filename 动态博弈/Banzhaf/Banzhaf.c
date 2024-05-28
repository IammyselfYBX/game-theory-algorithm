#include <stdio.h>
#include <stdlib.h>

// 用于存储每个投票者的信息
typedef struct {
    int weight;     // 投票权重
    int critical;   // 关键次数
    double rate;       // 班扎夫权力指数比数
} Voter;

// 函数原型声明
void calculateBanzhaf(Voter voters[], int n, int q);

int main() {
    int n, q;
    printf("请输入投票者数量n: ");
    scanf("%d", &n);
    printf("请输入必要票数q: ");
    scanf("%d", &q);
    Voter voters[n];
    for (int i = 0; i < n; i++) {
        printf("请输入第%d个投票者的权重: ", i+1);
        scanf("%d", &voters[i].weight);
        voters[i].critical = 0;
        voters[i].rate = 0;
    }

    calculateBanzhaf(voters, n, q);

    // 打印每个投票者的Banzhaf指数以及Banzhaf Ratio
    for (int i = 0; i < n; i++) {
        printf("第%d个投票者\n 班扎法权力指数(Banzhaf Index) = %.2f, 班扎夫权力指数比数(Banzhaf Ratio) = %.2f\n", i+1,(float)voters[i].critical , (float)voters[i].rate);
    }

    return 0;
}

void calculateBanzhaf(Voter voters[], int n, int q) {
    int pow_set_size = 1 << n;          // 2^n，即所有可能的投票组合数
    int count, total_critical = 0;

    /**
     * 遍历所有可能的投票组合
     *  n=3,即存在3个2进制位置  _ _ _ 
     * 对于3个投票者(n=3)，可能的组合为：000, 001, 010, 011, 100, 101, 110, 111 (2^n个)
     * 
     * j表示第j个投票者是否在当前组合中，换句话说就是在 n 个2进制位中第 j 位是否为 1
     * eg:i = 3 = 011，表示第1、2个投票者在当前组合中，第3个不在
    */
    for (int i = 0; i < pow_set_size; i++) { // 遍历所有可能的投票组合
        int sum = 0;

        // 计算在 i 的情况下，都包含哪些投票者 j
        // 并计算这些投票者的权重总和
        for (int j = 0; j < n; j++) {
            if (i & (1 << j)) { 
                sum += voters[j].weight;
            }
        }

        // 判断是否移除某个投票者 j 后无法通过
        for (int j = 0; j < n; j++) {
            if (i & (1 << j)) {     // 判断第j个投票者是否在当前组合中
                if (sum >= q && sum - voters[j].weight < q) {
                    voters[j].critical++; // 如果移除j后无法通过，则j是关键投票者
                    total_critical++;
                }
            }
        }
    }

    // 计算并打印每个投票者的标准化Banzhaf指数
    for (int i = 0; i < n; i++) {
        if (total_critical > 0) {
            voters[i].rate = (float)voters[i].critical / total_critical * 100.0;
        }
    }
}