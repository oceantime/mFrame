// Test: fillRect - Basic rectangle filling
// 测试基础矩形填充功能

function testFillRect(canvasId) {
    const canvas = document.getElementById(canvasId);
    if (!canvas) {
        console.error('Canvas not found:', canvasId);
        return false;
    }

    const ctx = canvas.getContext('2d');
    if (!ctx) {
        console.error('Cannot get 2d context');
        return false;
    }

    try {
        // Clear canvas
        ctx.clearRect(0, 0, canvas.width, canvas.height);

        // Test 1: Red rectangle
        ctx.fillStyle = '#FF0000';
        ctx.fillRect(10, 10, 100, 100);

        // Test 2: Green rectangle with transparency
        ctx.fillStyle = 'rgba(0, 255, 0, 0.5)';
        ctx.fillRect(60, 60, 100, 100);

        // Test 3: Blue rectangle
        ctx.fillStyle = '#0000FF';
        ctx.fillRect(110, 110, 100, 100);

        // Test 4: Yellow rectangle
        ctx.fillStyle = 'yellow';
        ctx.fillRect(160, 160, 80, 80);

        console.log('✓ fillRect test passed');
        return true;
    } catch (e) {
        console.error('✗ fillRect test failed:', e);
        return false;
    }
}

// Export for test runner
if (typeof module !== 'undefined' && module.exports) {
    module.exports = testFillRect;
}
