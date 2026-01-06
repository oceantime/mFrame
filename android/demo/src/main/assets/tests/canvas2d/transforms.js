// Test: transforms - Translate, rotate, scale
// 测试变换功能

function testTransforms(canvasId) {
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

        // Test 1: Translate
        ctx.save();
        ctx.fillStyle = '#FF0000';
        ctx.translate(50, 50);
        ctx.fillRect(0, 0, 40, 40);
        ctx.restore();

        // Test 2: Rotate
        ctx.save();
        ctx.fillStyle = '#00FF00';
        ctx.translate(150, 50);
        ctx.rotate(Math.PI / 4); // 45 degrees
        ctx.fillRect(-20, -20, 40, 40);
        ctx.restore();

        // Test 3: Scale
        ctx.save();
        ctx.fillStyle = '#0000FF';
        ctx.translate(250, 50);
        ctx.scale(1.5, 1.5);
        ctx.fillRect(-20, -20, 40, 40);
        ctx.restore();

        // Test 4: Combined transforms
        ctx.save();
        ctx.fillStyle = '#FF00FF';
        ctx.translate(350, 50);
        ctx.rotate(Math.PI / 6);
        ctx.scale(0.8, 1.2);
        ctx.fillRect(-20, -20, 40, 40);
        ctx.restore();

        // Test 5: Nested transforms
        ctx.save();
        ctx.fillStyle = 'rgba(255, 200, 0, 0.7)';
        ctx.translate(50, 150);
        
        for (let i = 0; i < 8; i++) {
            ctx.rotate(Math.PI / 4);
            ctx.fillRect(0, 0, 60, 10);
        }
        ctx.restore();

        // Test 6: Transform with paths
        ctx.save();
        ctx.strokeStyle = '#00FFFF';
        ctx.lineWidth = 3;
        ctx.translate(250, 200);
        ctx.rotate(Math.PI / 6);
        ctx.beginPath();
        ctx.arc(0, 0, 40, 0, Math.PI * 2);
        ctx.stroke();
        ctx.restore();

        console.log('✓ transforms test passed');
        return true;
    } catch (e) {
        console.error('✗ transforms test failed:', e);
        return false;
    }
}

if (typeof module !== 'undefined' && module.exports) {
    module.exports = testTransforms;
}
