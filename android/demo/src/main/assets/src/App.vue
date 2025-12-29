<template>
  <div class="app-container">
    <nav class="nav-bar">
      <div class="nav-title">🎨 Canvas演示</div>
      <div class="nav-buttons">
        <router-link to="/list" class="nav-btn" active-class="active">📋 列表</router-link>
        <router-link to="/picture" class="nav-btn" active-class="active">📷 相机</router-link>
        <router-link to="/gcanvas" class="nav-btn" active-class="active">🎯 GCanvas</router-link>
      </div>
    </nav>
    <div class="view-container">
      <router-view />
    </div>
  </div>
</template>

<script>
export default {
  name: 'App'
}
</script>

<style>
* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}

body {
  font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', 'Roboto', 'Helvetica Neue', Arial, sans-serif;
  background: #f5f5f5;
  overflow: hidden;
}

#app {
  width: 100vw;
  height: 100vh;
}

.app-container {
  display: flex;
  flex-direction: column;
  height: 100vh;
}

.nav-bar {
  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
  color: white;
  padding: 12px 20px;
  display: flex;
  align-items: center;
  justify-content: space-between;
  box-shadow: 0 2px 8px rgba(0,0,0,0.1);
  flex-shrink: 0;
}

.nav-title {
  font-size: 18px;
  font-weight: bold;
}

.nav-buttons {
  display: flex;
  gap: 8px;
}

.nav-btn {
  padding: 8px 16px;
  background: rgba(255, 255, 255, 0.2);
  color: white;
  text-decoration: none;
  border-radius: 6px;
  font-size: 14px;
  transition: all 0.3s ease;
  border: 1px solid rgba(255, 255, 255, 0.3);
}

.nav-btn:hover {
  background: rgba(255, 255, 255, 0.3);
  transform: translateY(-2px);
}

.nav-btn.active {
  background: white;
  color: #667eea;
  font-weight: 600;
}

.view-container {
  flex: 1;
  overflow: auto;
  background: #f5f5f5;
}
</style>
