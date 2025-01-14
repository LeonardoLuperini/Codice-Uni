#include "tsqueue.h"

node_t *node_init(void *data) {
    node_t *newnode = malloc(sizeof(node_t));
    if (newnode != NULL) {
        newnode->data = data;
        newnode->next = NULL;
    }
    return newnode;
}

queue_t *queue_init() {
    queue_t *q = malloc(sizeof(queue_t));
    ERR_RET(q == NULL, NULL);
    q->head = NULL;
    q->tail = NULL;
    mtx_init(&q->mtx);
    if (pthread_cond_init(&q->cond, NULL) != 0) {
        mtx_destroy(&q->mtx);
        free(q);
        return NULL;
    }

    return q;
}

bool queue_push(queue_t *queue, void *data) {
    node_t *newnode = node_init(data);
    if (newnode != NULL) {
        mtx_lock(&queue->mtx);
        if (queue->head == NULL)
            queue->head = newnode;
        if (queue->tail != NULL)
            queue->tail->next = newnode;
        queue->tail = newnode;
        cond_signal(&queue->cond);
        mtx_unlock(&queue->mtx);
        return true;
    }
    return false;
}

void *queue_pop(queue_t *queue) {
    void *data;
    node_t *old_node;
    mtx_lock(&queue->mtx);
    while (queue->tail == NULL)
        cond_wait(&queue->cond, &queue->mtx);
    data = queue->head->data;
    if (queue->tail == queue->head)
        queue->tail = NULL;
    old_node = queue->head;
    queue->head = queue->head->next;
    mtx_unlock(&queue->mtx);
    free(old_node);
    return data;
}

void queue_destroy(queue_t *queue) {
    node_t *node_to_free;
    while (queue->tail != NULL) {
        if (queue->tail == queue->head)
            queue->tail = NULL;
        node_to_free = queue->head;
        queue->head = queue->head->next;
        free(node_to_free);
    }
    mtx_destroy(&queue->mtx);
    cond_destroy(&queue->cond);
    free(queue);
}

void queue_destroy_fd(queue_t *queue) {
    node_t *node_to_free;
    while (queue->tail != NULL) {
        if (queue->tail == queue->head)
            queue->tail = NULL;
        node_to_free = queue->head;
        queue->head = queue->head->next;
        free(node_to_free->data);
        free(node_to_free);
    }
    mtx_destroy(&queue->mtx);
    cond_destroy(&queue->cond);
    free(queue);
}
