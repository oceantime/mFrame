import { createRouter, createWebHashHistory } from 'vue-router'
import ListView from '@/views/list/list.vue'
import PictureView from '@/views/picture/picture.vue'
import CanvasView from '@/views/canvas/canvas.vue'

const routes = [
    {
        path: '/',
        redirect: '/list'
    },
    {
        path: '/list',
        name: 'List',
        component: ListView
    },
    {
        path: '/picture',
        name: 'Picture',
        component: PictureView
    },
    {
        path: '/canvas',
        name: 'WmCanvas',
        component: CanvasView
    }
]

const router = createRouter({
    history: createWebHashHistory(),
    routes
})

export default router
