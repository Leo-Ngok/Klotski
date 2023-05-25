#include "chessboard.h"
#include <complex>
#include <QPainter>
#include <QPainterPath>
typedef std::complex<double> cplx;



ChessBoard::ChessBoard(QWidget *parent)
    : QWidget{parent}
{

}
void DisplayLine(QPainter *, cplx z_origin, cplx z_dest);
// Draw the chessboard
// Diagram
// A Classic Klotski is 5x4
// +----+----+----+----+-----> X
// |    |    |    |    |
// +----+----+----+----+
// |    |    |    |    |
// +----+----+----+----+
// |    |    |    |    |
// +----+----+----+----+
// |    |    |    |    |
// +----+----+----+----+
// |    |    |    |    |
// +----+----+----+----+
// |
// V Y
void ChessBoard::paintEvent(QPaintEvent *) {

    // Step one: paint the background color of the chessboard
    cplx top_left(TOP_LEFT_X, TOP_LEFT_Y);
    QPainter painter(this);
    QPainterPath path;
    // Start painter at top left corner
    path.moveTo(top_left.real(), top_left.imag());
    // Move to bottom left
    path.lineTo(top_left.real() + DIM_X * EDGE_LEN, top_left.imag());
    // Move to bottom right
    path.lineTo(top_left.real() + DIM_X * EDGE_LEN,
                top_left.imag() + DIM_Y * EDGE_LEN);
    // Move to top right
    path.lineTo(top_left.real(),
                top_left.imag() + DIM_Y * EDGE_LEN);
    // Move back to top left
    path.lineTo(top_left.real(), top_left.imag());
    // Now, a rectangle is drawn, now let's paint that rectangle
    // But first, let's set up the brush
    QBrush brush;
    QColor bgcolor(BG_R, BG_G, BG_B);
    brush.setColor(bgcolor);
    brush.setStyle(Qt::SolidPattern);
    // Now everything is ready, paint the rectangle.
    painter.fillPath(path, brush);

    // Step two: draw the grids.

    // Draw vertical grid lines

    // from base line to end line
    // Note: total length, rather than length of single grid.
    cplx vertical_offset(0, DIM_Y * EDGE_LEN);
    cplx horizontal_grid_offset(EDGE_LEN, 0);
    for(int i = 0; i < DIM_X + 1; ++i) {
        DisplayLine(&painter,
                    top_left + horizontal_grid_offset * (i + 0.0),
                    top_left + horizontal_grid_offset * (i + 0.0) +
                    vertical_offset);
    }
    // Draw horizontal grid lines
    cplx horizontal_offset(DIM_X * EDGE_LEN, 0);
    cplx vertical_grid_offset(0, EDGE_LEN);
    for(int i = 0; i < DIM_Y + 1; ++i) {
        DisplayLine(&painter,
                    top_left + vertical_grid_offset * (i + 0.0),
                    top_left + vertical_grid_offset * (i + 0.0) +
                    horizontal_offset);
    }
}


// Auxillary function for line construction

void DisplayLine(QPainter * painter, cplx z_origin, cplx z_dest) {
    QLineF ln(z_origin.real(), z_origin.imag(), z_dest.real(), z_dest.imag());
    painter->drawLine(ln);
}
