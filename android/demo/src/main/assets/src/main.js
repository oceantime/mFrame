import { createApp } from 'vue'
import App from './App.vue'
import router from './router'

console.log('=== 开始初始化Vue应用 ===')
console.log('Vue version:', createApp.version)

const app = createApp(App)
app.use(router)
app.mount('#app')

console.log('✅ Vue应用已启动')
