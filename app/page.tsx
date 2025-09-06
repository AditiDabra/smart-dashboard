import { DashboardCard } from "@/components/Dashboard";
import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card";

export default function Home() {
  return (
    <div className="p-6 space-y-6">
      {/* Top Section */}
      <div>
        <h1 className="text-2xl font-bold tracking-tight">Smart IIoT Dashboard</h1>
        <p className="text-muted-foreground">
          Monitor your devices and alerts in real-time.
        </p>
      </div>

      {/* Stats Cards */}
      <div className="grid gap-6 md:grid-cols-3">
        <DashboardCard
          title="Total Devices"
          value={11}
          description="Total devices in the system"
        />
        <DashboardCard
          title="Recent Alerts"
          value={0}
          description="Alerts in last 24 hours"
        />
        <DashboardCard
          title="Active Devices"
          value={1}
          description="Devices currently online"
        />
      </div>

      {/* Alerts Section */}
      <Card className="shadow-md">
        <CardHeader>
          <CardTitle className="text-lg font-semibold">Recent Alerts</CardTitle>
        </CardHeader>
        <CardContent className="h-48 flex items-center justify-center text-sm text-muted-foreground border-t">
          No alerts found 🚫
        </CardContent>
      </Card>
    </div>
  );
}
