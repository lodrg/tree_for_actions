#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TreeNode {
    char *action;
    struct TreeNode *children[10];
    int child_count;
    int is_leaf;
} TreeNode;

TreeNode* create_node(char *action) {
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    node->action = action ? strdup(action) : NULL;
    node->child_count = 0;
    node->is_leaf = 0;
    for (int i = 0; i < 10; i++) {
        node->children[i] = NULL;
    }
    return node;
}

TreeNode* add_child(TreeNode *node, char *child_action) {
    for (int i = 0; i < node->child_count; i++) {
        if (strcmp(node->children[i]->action, child_action) == 0) {
            return node->children[i];
        }
    }
    TreeNode *child_node = create_node(child_action);
    node->children[node->child_count++] = child_node;
    return child_node;
}

void print_tree(TreeNode *node, int level) {
    for (int i = 0; i < level; i++) {
        printf(i == level - 1 ? "|-- " : "|   ");
    }
    printf("%s\n", node->action ? node->action : "root");
    for (int i = 0; i < node->child_count; i++) {
        print_tree(node->children[i], level + 1);
    }
    if (node->is_leaf) {
        for (int i = 0; i < level + 1; i++) {
            printf(i == level ? "|-- " : "|   ");
        }
        printf("[LEAF]\n");
    }
}

typedef struct {
    char *action;
    int frequency;
} ActionFreq;

char *config[][5] = {
    {"action1", "action2", "action3", NULL, NULL},
    {"action2", "action4", "action5", NULL, NULL},
    {"action1", "action3", "action4", NULL, NULL},
    {"action2", "action3", NULL, NULL, NULL},
    {"action1", "action2", "action3", "action5", NULL}
};

int config_lengths[] = {3, 3, 3, 2, 4};

ActionFreq action_freq[10];
int freq_count = 0;

void count_action_freq(char *action) {
    for (int i = 0; i < freq_count; i++) {
        if (strcmp(action_freq[i].action, action) == 0) {
            action_freq[i].frequency++;
            return;
        }
    }
    action_freq[freq_count].action = action;
    action_freq[freq_count].frequency = 1;
    freq_count++;
}

int compare_action_freq(const void *a, const void *b) {
    return ((ActionFreq *)b)->frequency - ((ActionFreq *)a)->frequency;
}

void count_actions() {
    for (int i = 0; i < sizeof(config) / sizeof(config[0]); i++) {
        for (int j = 0; j < config_lengths[i]; j++) {
            count_action_freq(config[i][j]);
        }
    }
    qsort(action_freq, freq_count, sizeof(ActionFreq), compare_action_freq);
}

int get_action_freq(char *action) {
    for (int i = 0; i < freq_count; i++) {
        if (strcmp(action_freq[i].action, action) == 0) {
            return action_freq[i].frequency;
        }
    }
    return 0;
}

void sort_actions(char **actions, int length) {
    for (int i = 0; i < length - 1; i++) {
        for (int j = i + 1; j < length; j++) {
            if (get_action_freq(actions[i]) < get_action_freq(actions[j])) {
                char *temp = actions[i];
                actions[i] = actions[j];
                actions[j] = temp;
            }
        }
    }
}

void build_tree(TreeNode *node, char **action_group, int length) {
    if (length == 0) {
        node->is_leaf = 1;
        return;
    }
    char *current_action = action_group[0];
    TreeNode *child_node = add_child(node, current_action);
    build_tree(child_node, action_group + 1, length - 1);
}

int main() {
    TreeNode *root = create_node(NULL);

    // 统计动作频率
    count_actions();

    // 对每个路径进行排序
    for (int i = 0; i < sizeof(config) / sizeof(config[0]); i++) {
        sort_actions(config[i], config_lengths[i]);
    }

    // 构建树形结构
    for (int i = 0; i < sizeof(config) / sizeof(config[0]); i++) {
        build_tree(root, config[i], config_lengths[i]);
    }

    // 打印树结构
    print_tree(root, 0);

    return 0;
}