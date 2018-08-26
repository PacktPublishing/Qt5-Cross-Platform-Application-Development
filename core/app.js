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

function draw_grid()
{
    var x = this_player.x - width/2;  // x start point of the field
    var y = this_player.y - height/2;  // y start point of the field
    var absolute_x, absolute_y;

    context.lineWidth = 1;
    context.beginPath();
    for(var i = 0; i * gridSize < height; i++) { // draw the horizontal lines
        absolute_x = x + this_player.x;
        if (absolute_x <= 0 || absolute_x > 1000)
            continue;
       context.moveTo(x, i * gridSize + y);
       context.lineTo(x + width, i * gridSize + y);
    }
    for(i = 0; i * gridSize < width; i++) {  // draw the vertical lines
        absolute_y = y + this_player.y;
        if (absolute_y <= 0 || absolute_y > 1000)
            continue;
       context.moveTo(i * gridSize + x,  y);
       context.lineTo(i * gridSize + x, y + height);
    }
    context.stroke();

}

function draw_viruses(context, viruses, this_player)
{
    // console.log(viruses.length);
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

function draw_food(context, feed, this_player)
{
    for (var i = 0; i < feed.length; i++)
    {
        var food = feed[i];

        if (!food.enabled)
            continue;
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

function _draw_player_cells_helper(context, player, this_player)
{
    for (var cell_number=0; cell_number < player.cells.length; cell_number++)
    {
        var cell = player.cells[cell_number];
        var x_y = translate(cell, this_player, true);
        context.beginPath();
        context.arc(x_y[0],
                    x_y[1],
                    cell.radius,
                    0, 2*Math.PI);

        context.fill();
    }
}

function draw_players(context, players, this_player)
{
    for (var i=0; i < players.length; i++)
    {
        var player = players[i];
        context.fillStyle = player.hue;
        _draw_player_cells_helper(context, player, this_player)
        /* var cell = players[z];
        context.fillStyle = cell.hue;
        var x_y = translate(cell, this_player, true);
        context.beginPath();
        context.arc(x_y[0],
                    x_y[1],
                    cell.radius,
                    0, 2*Math.PI);

        context.fill();
        */
    }
}
