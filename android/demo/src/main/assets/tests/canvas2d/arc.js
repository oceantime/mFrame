// Test: arc - Circle and arc drawing
// 测试圆形和弧线绘制功能

function testArc(canvasId) {
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

        // Test 1: Full circle - filled
        ctx.fillStyle = '#FF0000';
        ctx.beginPath();
        ctx.arc(60, 60, 40, 0, Math.PI * 2);
        ctx.fill();

        // Test 2: Full circle - stroked
        ctx.strokeStyle = '#00FF00';
        ctx.lineWidth = 3;
        ctx.beginPath();
        ctx.arc(180, 60, 40, 0, Math.PI * 2);
        ctx.stroke();

        // Test 3: Half circle (arc)
        ctx.fillStyle = '#0000FF';
        ctx.beginPath();
        ctx.arc(60, 180, 40, 0, Math.PI);
        ctx.fill();

        // Test 4: Quarter circle
        ctx.strokeStyle = '#FF00FF';
        ctx.lineWidth = 3;
        ctx.beginPath();
        ctx.arc(180, 180, 40, 0, Math.PI / 2);
        ctx.stroke();

        // Test 5: Pac-Man shape
        ctx.fillStyle = '#FFFF00';
        ctx.beginPath();
        ctx.arc(300, 120, 50, Math.PI * 0.25, Math.PI * 1.75);
        ctx.lineTo(300, 120);
        ctx.closePath();
        ctx.fill();

        console.log('✓ arc test passed');
        return true;
    } catch (e) {
        console.error('✗ arc test failed:', e);
        return false;
    }
}

if (typeof module !== 'undefined' && module.exports) {
    module.exports = testArc;
}
