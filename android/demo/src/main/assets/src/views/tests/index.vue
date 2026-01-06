<template>
  <div class="tests-container">
    <div class="header">
      <h1>WmCanvas Tests</h1>
      <p class="subtitle">Canvas 2D & WebGL API Testing Suite</p>
    </div>

    <div class="test-options">
      <button @click="openTestPage" class="primary-btn">
        <span class="icon">🧪</span>
        Open Test Page
      </button>
      <button @click="runInlineTests" class="secondary-btn">
        <span class="icon">▶️</span>
        Run Tests Here
      </button>
    </div>

    <div v-if="showResults" class="results-summary">
      <h2>Test Results</h2>
      <div class="stats">
        <div class="stat-item passed">
          <div class="stat-value">{{ results.passed }}</div>
          <div class="stat-label">Passed</div>
        </div>
        <div class="stat-item failed">
          <div class="stat-value">{{ results.failed }}</div>
          <div class="stat-label">Failed</div>
        </div>
        <div class="stat-item total">
          <div class="stat-value">{{ results.total }}</div>
          <div class="stat-label">Total</div>
        </div>
      </div>
    </div>

    <div class="test-list">
      <h2>Available Tests</h2>
      
      <div class="test-category">
        <h3>Canvas 2D Tests</h3>
        <ul class="test-items">
          <li class="test-item">
            <span class="test-name">fillRect</span>
            <span class="test-desc">Basic rectangle filling</span>
          </li>
          <li class="test-item">
            <span class="test-name">strokeRect</span>
            <span class="test-desc">Rectangle stroking and borders</span>
          </li>
          <li class="test-item">
            <span class="test-name">arc</span>
            <span class="test-desc">Circle and arc drawing</span>
          </li>
          <li class="test-item">
            <span class="test-name">text</span>
            <span class="test-desc">Text rendering (fill & stroke)</span>
          </li>
          <li class="test-item">
            <span class="test-name">transforms</span>
            <span class="test-desc">Translate, rotate, scale</span>
          </li>
        </ul>
      </div>

      <div class="test-category">
        <h3>WebGL Tests</h3>
        <ul class="test-items">
          <li class="test-item disabled">
            <span class="test-name">triangle</span>
            <span class="test-desc">Basic triangle rendering (Coming soon)</span>
          </li>
          <li class="test-item disabled">
            <span class="test-name">cube</span>
            <span class="test-desc">3D cube with rotation (Coming soon)</span>
          </li>
          <li class="test-item disabled">
            <span class="test-name">texture</span>
            <span class="test-desc">Texture mapping (Coming soon)</span>
          </li>
        </ul>
      </div>
    </div>

    <div class="info-box">
      <h3>📖 About Tests</h3>
      <p>
        These tests verify the WmCanvas implementation against standard Canvas 2D
        and WebGL APIs. Each test exercises specific functionality and reports results.
      </p>
      <p>
        Tests are adapted from the Node.js examples in <code>node/examples/</code> and
        verify compatibility with the native C++ implementation.
      </p>
    </div>
  </div>
</template>

<script>
export default {
  name: 'TestsView',
  data() {
    return {
      showResults: false,
      results: {
        total: 5,
        passed: 0,
        failed: 0
      }
    };
  },
  methods: {
    openTestPage() {
      // Open tests/index.html in WebView
      window.location.href = '/tests/index.html';
    },
    runInlineTests() {
      // TODO: Implement inline test runner
      this.$message({
        type: 'info',
        message: 'Inline test runner will be implemented in next phase'
      });
      
      // For now, redirect to test page
      this.openTestPage();
    }
  }
};
</script>

<style scoped>
.tests-container {
  padding: 20px;
  max-width: 900px;
  margin: 0 auto;
}

.header {
  text-align: center;
  margin-bottom: 30px;
}

.header h1 {
  font-size: 32px;
  color: #333;
  margin-bottom: 8px;
}

.subtitle {
  font-size: 16px;
  color: #666;
}

.test-options {
  display: flex;
  gap: 15px;
  justify-content: center;
  margin-bottom: 30px;
}

.primary-btn,
.secondary-btn {
  padding: 12px 24px;
  border: none;
  border-radius: 6px;
  font-size: 16px;
  cursor: pointer;
  display: flex;
  align-items: center;
  gap: 8px;
  transition: all 0.3s;
}

.primary-btn {
  background: #007bff;
  color: white;
}

.primary-btn:hover {
  background: #0056b3;
  transform: translateY(-2px);
  box-shadow: 0 4px 8px rgba(0, 123, 255, 0.3);
}

.secondary-btn {
  background: #6c757d;
  color: white;
}

.secondary-btn:hover {
  background: #5a6268;
  transform: translateY(-2px);
  box-shadow: 0 4px 8px rgba(108, 117, 125, 0.3);
}

.icon {
  font-size: 18px;
}

.results-summary {
  background: #f8f9fa;
  border-radius: 8px;
  padding: 20px;
  margin-bottom: 30px;
}

.results-summary h2 {
  margin-top: 0;
  margin-bottom: 20px;
  color: #333;
}

.stats {
  display: flex;
  gap: 20px;
  justify-content: space-around;
}

.stat-item {
  text-align: center;
  padding: 20px;
  border-radius: 8px;
  min-width: 120px;
}

.stat-item.passed {
  background: #d4edda;
}

.stat-item.failed {
  background: #f8d7da;
}

.stat-item.total {
  background: #d1ecf1;
}

.stat-value {
  font-size: 36px;
  font-weight: bold;
  margin-bottom: 5px;
}

.stat-item.passed .stat-value {
  color: #155724;
}

.stat-item.failed .stat-value {
  color: #721c24;
}

.stat-item.total .stat-value {
  color: #0c5460;
}

.stat-label {
  font-size: 14px;
  color: #666;
}

.test-list {
  margin-bottom: 30px;
}

.test-list > h2 {
  font-size: 24px;
  color: #333;
  margin-bottom: 20px;
}

.test-category {
  margin-bottom: 25px;
}

.test-category h3 {
  font-size: 18px;
  color: #555;
  margin-bottom: 12px;
  padding-bottom: 8px;
  border-bottom: 2px solid #e0e0e0;
}

.test-items {
  list-style: none;
  padding: 0;
  margin: 0;
}

.test-item {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 12px 15px;
  margin-bottom: 8px;
  background: white;
  border: 1px solid #e0e0e0;
  border-radius: 6px;
  transition: all 0.2s;
}

.test-item:hover:not(.disabled) {
  border-color: #007bff;
  box-shadow: 0 2px 4px rgba(0, 123, 255, 0.1);
  transform: translateX(5px);
}

.test-item.disabled {
  opacity: 0.5;
  cursor: not-allowed;
}

.test-name {
  font-weight: 600;
  color: #007bff;
  font-family: 'Courier New', monospace;
}

.test-item.disabled .test-name {
  color: #999;
}

.test-desc {
  color: #666;
  font-size: 14px;
}

.info-box {
  background: #e7f3ff;
  border-left: 4px solid #007bff;
  padding: 20px;
  border-radius: 4px;
}

.info-box h3 {
  margin-top: 0;
  color: #0056b3;
}

.info-box p {
  margin: 10px 0;
  line-height: 1.6;
  color: #333;
}

.info-box code {
  background: rgba(0, 0, 0, 0.05);
  padding: 2px 6px;
  border-radius: 3px;
  font-family: 'Courier New', monospace;
}
</style>
