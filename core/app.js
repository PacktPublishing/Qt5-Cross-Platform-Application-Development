function _draw_circle(context, centerX, centerY, radius, sides) {
    context.beginPath();

    for (var i = 0; i < sides; i++) {
        var theta = (i / sides) * 2 * Math.PI;
        var x = centerX + radius * Math.sin(theta);
        var y = centerY + radius * Math.cos(theta);
        context.lineTo(x, y);
    }

    context.closePath();
    context.stroke();
    context.fill();
}

function draw_objects(context, feed, players, viruses, this_player, window_width, window_height)
{
    draw_grid(context, this_player, window_width, window_height);
    draw_food(context, feed, this_player, window_width, window_height);
    draw_players(context, players, this_player, window_width, window_height);
    draw_viruses(context, viruses, this_player, window_width, window_height);
}

function translate(object, this_player)
{
    var relative_x = object.x + (width/2);
    var relative_y = object.y + (height/2);
    // var scaled_x = relative_x * this_player.zoom_factor;
    // var scaled_y = relative_y * this_player.zoom_factor;
    var scaled_x = relative_x;
    var scaled_y = relative_y;
    relative_x = scaled_x - this_player.x;
    relative_y = scaled_y - this_player.y;
    return [relative_x, relative_y];
}

function draw_grid(context, this_player, width, height)
{
    context.lineWidth = 1;
    context.strokeStyle = "#000000"
    context.globalAlpha = 0.15
    // Left-vertical
    if (this_player.x <= width/2) {
        context.beginPath();
        context.moveTo(width/2 - this_player.x, 0 ? this_player.y > height/2 : height/2 - this_player.y);
        context.lineTo(width/2 - this_player.x, 1000 + height/2 - this_player.y);
        context.stroke();
    }

    // Top-horizontal.
    if (this_player.y <= height/2) {
        context.beginPath();
        context.moveTo(0 ? this_player.x > width/2 : width/2 - this_player.x,height/2 - this_player.y);
        context.lineTo(1000 + width/2 - this_player.x, height/2 - this_player.y);
        context.stroke();
    }

    // Right-vertical.
    if (1000 - this_player.x <= width/2) {
        context.beginPath();
        context.moveTo(1000+ width/2 - this_player.x,
                     height/2 - this_player.y);
        context.lineTo(1000+ width/2 - this_player.x,
                     1000+ height/2 - this_player.y);
        context.stroke();
    }

    // Bottom-horizontal.
    if (1000 - this_player.y <= height/2) {
        context.beginPath();
        context.moveTo(1000 + width/2 - this_player.x,
                     1000 + height/2 - this_player.y);
        context.lineTo(width/2 - this_player.x,
                     1000 + height/2 - this_player.y);
        context.stroke();
    }

    context.stroke();
    context.globalAlpha = 1;
}

function draw_viruses(context, viruses, this_player, width, height)
{
    for (var i = 0; i < viruses.length; i++)
    {
        var virus = viruses[i];
        var radius = virus.radius
        var x_y = translate(virus, this_player)

        var x = x_y[0];
        var y = x_y[1];

        if (x > width + radius || x < 0 - radius)
            continue;
        if (y > height + radius || y < 0 - radius)
            continue;

        context.fillStyle = "#33ff33"
        context.lineWidth = 10
        context.strokeStyle = "#19D119"
        _draw_circle(context, x, y, virus.radius, 12);
    }

}

function draw_food(context, feed, this_player, width, height)
{
    for (var i = 0; i < feed.length; i++)
    {
        var food = feed[i];
        var x_y = translate(food, this_player);

        var x = x_y[0];
        var y = x_y[1];
        if (x > width || x < 0)
            continue;
        if (y > height || y < 0)
            continue;
        context.beginPath();
        context.fillStyle = food.hue;
        context.arc(x,
                    y,
                    food.radius, 0, 2*Math.PI);

        context.fill();
    }
}

function _draw_player_cells_helper(context, player, this_player, width, height)
{
    for (var cell_number=0; cell_number < player.cells.length; cell_number++)
    {
        var cell = player.cells[cell_number];
        var x_y = translate(cell, this_player, true);
        var x = x_y[0];
        var y = x_y[1];
        if (x > width || x < 0)
            continue;
        if (y > height || y < 0)
            continue;
        context.beginPath();
        context.arc(x_y[0],
                    x_y[1],
                    cell.radius,
                    0, 2*Math.PI);

        context.fill();
    }
}

function draw_players(context, players, this_player, width, height)
{
    for (var i=0; i < players.length; i++)
    {
        var player = players[i];
        context.fillStyle = player.hue;
        _draw_player_cells_helper(context, player, this_player, width, height)
    }
}
