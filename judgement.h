#pragma once

bool RectangleIntersectCircle(int Rx, int Ry, int Rw, int Rh, int Cx, int Cy, int Cr) {
	int	closeRx = (Cx < Rx) ? (Rx) : ((Cx > Rx + Rw) ? (Rx + Rw) : (Cx)),
		closeRy = (Cy < Ry) ? (Ry) : ((Cy > Ry + Rh) ? (Ry + Rh) : (Cy));

	return ((closeRx - Cx) * (closeRx - Cx) + (closeRy - Cy) * (closeRy - Cy) <= Cr * Cr);
}

bool IntervalIntersectInterval(int x1, int w1, int x2, int w2) {
	return ((x1 <= x2 + w2) && (x2 <= x1 + w1));
}
