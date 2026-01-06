// Test: strokeRect - Rectangle stroking
// 测试矩形描边功能

function testStrokeRect(canvasId) {
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

        // Test 1: Basic stroke
        ctx.strokeStyle = '#FF0000';
        ctx.lineWidth = 2;
        ctx.strokeRect(10, 10, 100, 100);

        // Test 2: Thick stroke
        ctx.strokeStyle = '#00FF00';
        ctx.lineWidth = 5;
        ctx.strokeRect(120, 10, 100, 100);

        // Test 3: Different line cap and join
        ctx.strokeStyle = '#0000FF';
        ctx.lineWidth = 3;
        ctx.lineCap = 'round';
        ctx.lineJoin = 'round';
        ctx.strokeRect(230, 10, 100, 100);

        // Test 4: Dashed stroke (if supported)
        ctx.strokeStyle = '#FF00FF';
        ctx.lineWidth = 2;
        if (ctx.setLineDash) {
            ctx.setLineDash([5, 5]);
        }
        ctx.strokeRect(10, 120, 100, 100);

        console.log('✓ strokeRect test passed');
        return true;
    } catch (e) {
        console.error('✗ strokeRect test failed:', e);
        return false;
    }
}

if (typeof module !== 'undefined' && module.exports) {
    module.exports = testStrokeRect;
}
