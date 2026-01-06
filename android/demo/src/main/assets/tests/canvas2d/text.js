// Test: fillText and strokeText - Text rendering
// 测试文本渲染功能

function testText(canvasId) {
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

        // Test 1: Basic filled text
        ctx.fillStyle = '#000000';
        ctx.font = '24px sans-serif';
        ctx.fillText('Hello WmCanvas!', 10, 30);

        // Test 2: Different font size
        ctx.fillStyle = '#FF0000';
        ctx.font = '36px sans-serif';
        ctx.fillText('Large Text', 10, 80);

        // Test 3: Stroked text
        ctx.strokeStyle = '#0000FF';
        ctx.lineWidth = 2;
        ctx.font = '28px sans-serif';
        ctx.strokeText('Outline Text', 10, 130);

        // Test 4: Text alignment - left
        ctx.fillStyle = '#00FF00';
        ctx.font = '20px sans-serif';
        ctx.textAlign = 'left';
        ctx.fillText('Left Aligned', 150, 170);

        // Test 5: Text alignment - center
        ctx.textAlign = 'center';
        ctx.fillText('Center Aligned', 200, 200);

        // Test 6: Text alignment - right
        ctx.textAlign = 'right';
        ctx.fillText('Right Aligned', 250, 230);

        // Test 7: Text baseline - top
        ctx.fillStyle = '#FF00FF';
        ctx.textAlign = 'left';
        ctx.textBaseline = 'top';
        ctx.fillText('Top Baseline', 10, 260);

        // Test 8: Text baseline - middle
        ctx.textBaseline = 'middle';
        ctx.fillText('Middle Baseline', 10, 290);

        // Test 9: Text baseline - bottom
        ctx.textBaseline = 'bottom';
        ctx.fillText('Bottom Baseline', 10, 320);

        console.log('✓ text test passed');
        return true;
    } catch (e) {
        console.error('✗ text test failed:', e);
        return false;
    }
}

if (typeof module !== 'undefined' && module.exports) {
    module.exports = testText;
}
