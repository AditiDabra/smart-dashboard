import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card";

type Props = { title: string; value: string | number; description: string };

export function DashboardCard({ title, value, description }: Props) {
  return (
    <Card className="w-full shadow-sm">
      <CardHeader>
        <CardTitle className="text-base">{title}</CardTitle>
      </CardHeader>
      <CardContent>
        <p className="text-3xl font-semibold">{value}</p>
        <p className="text-sm text-muted-foreground">{description}</p>
      </CardContent>
    </Card>
  );
}
